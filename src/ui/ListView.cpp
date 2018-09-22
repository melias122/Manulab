#include "ListView.h"

ListView::ListView(QWidget *parent)
	: QListView(parent)
{
	setModel(&m_model);
}

void ListView::setStringList(const QStringList &list)
{
	m_model.setStringList(list);
}

QStringList ListView::stringList() const
{
	return m_model.stringList();
}
