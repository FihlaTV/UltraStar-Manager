#include "QUAmazonImageCollector.h"
#include "QUAmazonImageSource.h"

#include "QUCommunicatorInterface.h"
#include "QUSongInterface.h"

#include "QUSongSupport.h"

#include "QUAmazonRequestUrl.h"
#include "QUAmazonResponse.h"

#include <QHttp>
#include <QBuffer>
#include <QFile>
#include <QDomDocument>

QUAmazonImageCollector::QUAmazonImageCollector(QUSongInterface *song, QUAmazonImageSource *source): QUHttpCollector(song, source) {}

QURequestUrl* QUAmazonImageCollector::url() const {
	return new QUAmazonRequestUrl(
			source()->host(),
			QStringList() << source()->songDataField("artist") << source()->songDataField("title"),
			song());
}

void QUAmazonImageCollector::processSearchResults() {
	QDomDocument doc; doc.setContent(buffer()->data());
	QUAmazonResponse response(doc);

	if(!response.isValid()) {
		setState(Idle);
		communicator()->send(tr("Invalid request."));
		communicator()->send(QUCommunicatorInterface::Failed);
		return;
	}

	// remove old downloads, if necessary
	if(!source()->keepDownloads()) {
		QFileInfoList fiList = source()->imageFolder(song()).entryInfoList(QUSongSupport::allowedPictureFiles(), QDir::Files, QDir::Name);
		foreach(QFileInfo fi, fiList)
			QFile::remove(fi.filePath());
	}

	if(response.count() == 0) {
		setState(Idle);
		communicator()->send(tr("No results."));
		communicator()->send(QUCommunicatorInterface::Done);
		return;
	}

	setState(ImageRequest);

	for(int i = 0; i < response.count() and i < source()->limit(); i++) {
		QFile *file = openLocalFile(source()->imageFolder(song()).filePath(QFileInfo(response.url(i, QU::largeImage).path()).fileName()));

		if(file) {
			http()->setHost(response.url(i, QU::largeImage).host());
			http()->get(response.url(i, QU::largeImage).path(), file);
		}
	}
}