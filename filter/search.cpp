#include "search.h"
#include "settingsDialog.h"

QString Search::arg1 = "Phrase/Regex";

Search::Search()
{
	uiFlag = UiFlag(result | settings);
	argv[arg1] = "";

	header_view = new QHeaderView(Qt::Horizontal);
	le_file_path.setText(QDir::homePath() + QDir::separator());
	le_file_path.setPlaceholderText("Path to the folder");
	btn_open_file_browser.setText("Open file browser");
	btn_export_data.setText("Export to csv");

	sort_asc = true;
	connect(&btn_export_data, &QPushButton::clicked, this, &Search::export_result);
	connect(&btn_open_file_browser, &QPushButton::clicked, this, &Search::open_file_browser);
	connect(header_view, &QHeaderView::sectionDoubleClicked, this, &Search::sort_by_colum);
}

void Search::sort_by_colum(int column)
{
	if (sort_asc == true) {
		table.sortByColumn(column, Qt::AscendingOrder);
		sort_asc = false;
	} else {
		table.sortByColumn(column, Qt::DescendingOrder);
		sort_asc = true;
	}
}

QList<Page> Search::processPages(QList<Page> &pages)
{

	auto phrase = argv[arg1].toString();

	for (Page &page : pages) {
		quint32 pageNum = page.index() + 1;
		QString text = page.plainText();

		QList<QString> lines = text.split("\n");
		quint32 line = 1;
		foreach (QString l, lines) {
			if (l.contains(phrase)) {
				data.append(qMakePair(pageNum, line));
			}
			line++;
		}
	}

	return pages;
}

void Search::reset()
{
	data.clear();
}

QString Search::process(QString &text)
{
	return text;
}

void Search::settingsUi(QWidget *parent)
{
	settingsDialog dialog(parent);

	Args args = getArgs();
	dialog.addLineEdit(arg1, args[arg1]);

	if (dialog.exec() == QDialog::Accepted) {
		setArgs(dialog.args);
	}
}

void Search::resultUi(QWidget *parent)
{
	if (data.isEmpty()) {
		return;
	}

	// UI
	QDialog *dialog = new QDialog(parent);
	QGridLayout main_layout;

	// adding widgetss to layout
	main_layout.addWidget(&table, 0, 0, 1, 2);
	main_layout.addWidget(&le_file_path, 1, 0, 1, 1);
	main_layout.addWidget(&btn_open_file_browser, 1, 1, 1, 1);
	main_layout.addWidget(&btn_export_data, 2, 0, 1, 2);

	dialog->setLayout(&main_layout);
	auto phrase = argv[arg1].toString();

	dialog->setWindowTitle("Search result for phrase " + phrase);

	// setting rows, columns and labels for columns
	table.setRowCount(data.count());
	table.setColumnCount(2);
	table.setHorizontalHeaderLabels(QStringList() << "page"
												  << "line");

	// stretch the columns width to their content
	table.setHorizontalHeader(header_view);
	table.resizeColumnsToContents();

	// align the labels to left
	for (int i = 0; i < 2; i++) {
		table.horizontalHeaderItem(i)->setTextAlignment(Qt::AlignLeft);
	}

	// inserting data to table data representation
	for (int i = 0; i < data.count(); i++) {
		QPair<quint32, quint32> p = data.at(i);

		QTableWidgetItem *page = new QTableWidgetItem;
		page->setData(Qt::DisplayRole, QString::number(p.first));
		QTableWidgetItem *line = new QTableWidgetItem;
		line->setData(Qt::DisplayRole, QString::number(p.second));

		table.setItem(i, 0, page);
		table.setItem(i, 1, line);
	}

	dialog->exec();
}

QString Search::export_result(void)
{
	QString filePath = le_file_path.text();
	if (filePath.isEmpty()) {
		filePath = QDir::currentPath() + QDir::separator();
	}

	pallete.setColor(QPalette::Text, Qt::black);
	le_file_path.setPalette(pallete);

	if (!QDir(filePath).exists()) {
		pallete.setColor(QPalette::Text, Qt::red);
		le_file_path.setPalette(pallete);
		le_file_path.setText(filePath + " Folder not found");
		return "Folder not found";
	}

	QString time = QDateTime::currentDateTime().toString("yyyy_M_d_h_m");
	QString filename = Name + "_histogram_" + time + ".csv";

	QFile file(filePath + filename);
	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		qDebug() << "Could not open file for writing.";
		return "Could not open file for writing, check your permissions";
	}

	QTextStream stream(&file);
	stream.setCodec("UTF-8");

	auto phrase = argv[arg1].toString();
	stream << "searched for " << phrase << "\n";

	// data representation
	for (int i = 0; i < data.count(); ++i) {
		QPair<quint32, quint32> p = data.at(i);
		stream << "page: " << QString::number(p.first);
		stream << ", line: " << QString::number(p.second);
		stream << "\n";
	}

	file.flush();
	file.close();
	return "Successfully exported to file.";
}

void Search::open_file_browser()
{
	QString dir = QFileDialog::getExistingDirectory();

	if (!dir.isEmpty()) {
		dir.append(QDir::separator());
		pallete.setColor(QPalette::Text, Qt::black);
		le_file_path.setPalette(pallete);
		le_file_path.setText(dir);
	}
}
