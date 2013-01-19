#include <QtGui>

class QWebView;
class QLineEdit;

class BrowserWindow : public QMainWindow {
	Q_OBJECT

	public:
		BrowserWindow(const QUrl& url);
	
	private:
		QString fQuery;
		QWebView* fView;
		QLineEdit* fLocationEdit;
		QAction* fRotateAction;
		int fProgress;

	protected slots:
		void adjustLocation();
		void changeLocation();
		void adjustTitle();
		void setProgress(int p);
		void finishLoading(bool);
		
		void viewSource();
		void slotSourceDownloaded();

//		void hightlightAllLinks();
//		void rotateImages(bool invert);
//		void removeGifImages();
//		void removeInlineFrames();
//		void removeObjectElements();
//		void removeEmbeddedElements();
};
