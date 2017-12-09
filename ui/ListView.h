#ifndef LISTVIEW_H
#define LISTVIEW_H

#include "qlistview.h"
#include "qstringlistmodel.h"

class ListView : public QListView
{
	Q_OBJECT

public:
	ListView(QWidget *parent);
	void setStringList(const QStringList &list);
	QStringList stringList() const;

private:
	QStringListModel m_model;
};

#endif // LISTVIEW_H
