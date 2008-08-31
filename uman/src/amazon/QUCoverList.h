#ifndef QUCOVERLIST_H_
#define QUCOVERLIST_H_

#include "QUCoverModel.h"

#include <QListView>
#include <QString>

class QUCoverList: public QListView {
	Q_OBJECT

public:
	QUCoverList(QWidget *parent = 0);
	QString currentFilePath();

	QUCoverModel* model();

private slots:
	void passActivation(const QModelIndex &index);

signals:
	void coverActivated(const QString &filePath);

};

#endif /* QUCOVERLIST_H_ */
