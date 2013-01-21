#include <QtGui>
#include <QtWebKit>
#include "browserwindow.h"

BrowserWindow::BrowserWindow(const QUrl& url) {
	fProgress = 0;

	QNetworkProxyFactory::setUseSystemConfiguration(true);

	fView = new QWebView(this);
	fView->load(url);
	connect(fView, SIGNAL(loadFinished(bool)), SLOT(adjustLocation()));
	connect(fView, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
	connect(fView, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
	connect(fView, SIGNAL(loadFinished(bool)), SLOT(finishedLoading(bool)));

//	fLocationEdit = new QLineEdit(this);
//	fLocationEdit->setSizePolicy(QSizePolicy::Expanding, fLocationEdit->sizePolicy().verticalPolicy());
//	connect(fLocationEdit, SIGNAL(returnPressed()), SLOT(changeLocation()));

//	QToolBar* toolBar = addToolBar(tr("Navigation"));
//	toolBar->addAction(fView->pageAction(QWebPage::Back));
//	toolBar->addAction(fView->pageAction(QWebPage::Forward));
//	toolBar->addAction(fView->pageAction(QWebPage::Reload));
//	toolBar->addAction(fView->pageAction(QWebPage::Stop));
//	toolBar->addWidget(fLocationEdit);

//	QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
//	QAction* viewSourceAction = new QAction("Page Source", this);
//	connect(viewSourceAction, SIGNAL(triggered()), SLOT(viewSource()));
//	viewMenu->addAction(viewSourceAction);

	//.... missing fancy stuff from example
	
	setCentralWidget(fView);
	setUnifiedTitleAndToolBarOnMac(true);
}

//void BrowserWindow::viewSource() {
//	QNetworkAccessManager* accessManager = fView->page()->networkAccessManager();
//	QNetworkRequest request(fView->url());
//	QNetworkReply* reply = accessManager->get(request);
//	connect(reply, SIGNAL(finished()), this, SLOT(slotSourceDownloaded()));
//}
//
//void BrowserWindow::slotSourceDownloaded() {
//	QNetworkReply* reply = qobject_cast<QNetworkReply*>(const_cast<QObject*>(sender()));
//	QTextEdit* textEdit = new QTextEdit(NULL);
//	textEdit->setAttribute(Qt::WA_DeleteOnClose);
//	textEdit->show();
//	textEdit->setPlainText(reply->readAll());
//	reply->deleteLater();
//}

//void BrowserWindow::adjustLocation() {
//	fLocationEdit->setText(fView->url().toString());
//}

//void BrowserWindow::changeLocation() {
//	QUrl url(fLocationEdit->text());
//	fView->load(url);
//	fView->setFocus();
//}

void BrowserWindow::adjustTitle() {
	if (fProgress <= 0 || fProgress >= 100) {
		setWindowTitle(fView->title());
	}
	else {
		setWindowTitle(QString("%1 (%2%)").arg(fView->title()).arg(fProgress));
	}
}

void BrowserWindow::setProgress(int p) {
	fProgress = p;
	adjustTitle();
}

void BrowserWindow::finishLoading(bool) {
	setProgress(100);
//	fView->page()->mainFrame()->evaluateJavaScript(jQuer);
}
