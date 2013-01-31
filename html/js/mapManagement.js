var map=null;
var selectControl;
var busses;
var maxVehicles = 50; // maximum visualized vehicles is limited because of browser performance
var labelFontSize = 14;
var busArray = new Object();

var centerCoordinates;
var zoomLevel;

function movePoint(point, x, y) {
	point.x = x;
	point.y = y;
	point.clearBounds();
}

function OLinit() {

//	alert("OLinit()");
	map = new OpenLayers.Map("map");
	var mapnik = new OpenLayers.Layer.OSM();

	// vector map holding currently visualized busses
	busses=new OpenLayers.Layer.Vector(
		"Busses",
		{
			styleMap: new OpenLayers.StyleMap({
				'default': {
					strokeColor: "#000000",
					strokeOpacity: 1,
					strokeWidth: 2,
					fillColor: "${markerColor}",
					fillOpacity: 1,
					pointRadius: "${markerSize}",
					// label with \n linebreaks
					label : "${labelText}",
					externalGraphic: "${graphic}",
					graphicHeight: 24,
					rotation: "${angle}",
					fontColor: "black",
					fontSize: "${labelFontSize}px",
					fontFamily: "Arial",
					fontWeight: "bold",
					labelAlign: "cb",
					labelXOffset: "0",
					labelYOffset: "-32"
				},
				"select": {
					cursor: "crosshair",
					externalGraphic: "img/marker-blue.png"
				}
			})
		}
	);

	// adding MAP LAYERS
	map.addLayers([mapnik,busses]);

	selectControl = new OpenLayers.Control.SelectFeature(
		busses,
		{
			onSelect: onFeatureSelect,
			onUnselect: onFeatureUnselect,
			hover: false
		}
	);

//	map.addControl(new OpenLayers.Control.LayerSwitcher());
// 	map.addControl(new OpenLayers.Control.MousePosition());
	map.addControl(new OpenLayers.Control.PanZoomBar());
	map.addControl(selectControl);
	selectControl.activate();

	// initilazie map
	zoomLevel=6; // good overview over germany/central europe

	centerMap(51.0, 9.0);
}

function onFeatureSelect(feature) {
	selectedFeature = feature;

	//var planeAngle = feature.attributes.angle;

	popup = new OpenLayers.Popup.AnchoredBubble(
		null, // id
		new OpenLayers.LonLat(feature.geometry.x, feature.geometry.y), // lonlat
		null, // contentSize
		feature.attributes.text, // contentHTML
		null, // anchor
		true, // closeBoxe
		function () { onPopupClose(feature) }  // function to be called on closeBox click
	);
	popup.autoSize = true;
	feature.popup = popup;
	feature.attributes.poppedup=true;
	map.addPopup(popup);
}

function onPopupClose(feature) {
	selectControl.unselect(feature);
}

function onFeatureUnselect(feature) {
	map.removePopup(feature.popup);
	feature.popup.destroy();
	feature.attributes.poppedup = false;
	feature.popup = null;
}

function centerMap(lat, lng) {
	var coor = new OpenLayers.LonLat(lng, lat ).transform(
		new OpenLayers.Projection("EPSG:4326"), // transform from WGS 1984
		map.getProjectionObject() // to Spherical Mercator Projection
	);

	map.setCenter(coor, zoomLevel);
}

function setZoomLevel(zoom) {
	zoomLevel = zoom;

	map.zoomTo(zoom);
}

function setLabelFontSize(inFontSize) {
	labelFontSize = inFontSize;
}

function addMapMarker(id, lat, lng, course) {
	// add it to map
	var markerCoords = new OpenLayers.LonLat(lng, lat).transform(
		new OpenLayers.Projection("EPSG:4326"),
		map.getProjectionObject()
	);

//	alert(showLabel + ' --> Label: ' + strLabel);
	// add marker to map
	var bus = new OpenLayers.Feature.Vector(
		new OpenLayers.Geometry.Point(markerCoords.lon, markerCoords.lat),
		{
		angle: course,
		poppedup: false,
		}
	);

	busses.addFeatures([bus]);

	// also store it in the global busArray
	busArray[id] = bus;
	return 'done';
}

function test() {
	alert('Test');
	return 'Katze';
}

// create marker
function createMapMarker(id, lat, lng, course, html, strLabel, mSize, mColor) {
	// add it to map
	var markerCoords = new OpenLayers.LonLat(lng, lat).transform(
		new OpenLayers.Projection("EPSG:4326"),
		map.getProjectionObject()
	);

//	alert(showLabel + ' --> Label: ' + strLabel);
	// add marker to map
	var bus = new OpenLayers.Feature.Vector(
		new OpenLayers.Geometry.Point(markerCoords.lon, markerCoords.lat),
		{
		angle: course,
		poppedup: false,
		labelFontSize: labelFontSize,
		text: html,
		labelText: strLabel,
		markerSize: mSize,
		markerColor: mColor,
	    	graphic: mColor
		}
	);

	busses.addFeatures([bus]);

	// also store it in the global busArray
	busArray[id] = bus;
	return 'done';
}

// update marker
function updateMapMarker(id, lat, lng, course, html, showLabel, mSize, mColor) {
	// check if marker already existed
	var poppedUp = false;
	if (busArray[id] === undefined) {
	}
	else {
		poppedUp = busArray[id].attributes.poppedup;
		deleteMarker(id);
	}

	// ok marker does not exist --> create id
	createMapMarker(id, lat, lng, course, html, showLabel, mSize, mColor);

	if (poppedUp) onFeatureSelect(busArray[id]);
}

function deleteMarker(id) {
	if (busArray[id] === undefined) {
		// nothing to do
	}
	else {
		var bus = busArray[id];

		if (bus.attributes.poppedup) {
			onFeatureUnselect(bus);
		}

		busses.destroyFeatures([bus]);
		bus.destroy();
		
		delete busArray[id];
	}
}
