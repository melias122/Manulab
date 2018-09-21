#include "frequency.h"

#include <QFontMetrics>

#include "project/config.h"
#include "settingsDialog.h"

QString Frequency::arg1 = "N-gram";
QString Frequency::arg2 = "Delimitter";

Frequency::Frequency()
{
	uiFlag = UiFlag(result | settings);
	argv[arg1] = "1";
	ngram_len = 1;
	argv[arg2] = "";
	delimitter = "";

	header_view = new QHeaderView(Qt::Horizontal);
	le_file_path.setText(QDir::homePath() + QDir::separator());
	le_file_path.setPlaceholderText("Path to the folder");
	btn_open_file_browser.setText("Open file browser");
	btn_export_data.setText("Export to csv");
	btn_show_graph.setText("Show graph"); //new Button
	btn_option_abs.setText("absolute");
	btn_option_rel.setText("relative");
	btn_grp.addButton(&btn_option_abs);
	btn_grp.addButton(&btn_option_rel);
	btn_option_abs.setChecked(true);

	sort_asc = true;
	is_abs = true;

	connect(&btn_export_data, &QPushButton::clicked, this, &Frequency::export_histogram);
	connect(&btn_open_file_browser, &QPushButton::clicked, this, &Frequency::open_file_browser);
	connect(&btn_show_graph, &QPushButton::clicked, this, &Frequency::show_graph); //connect function, called after button press
	connect(&btn_option_abs, &QPushButton::clicked, this, &Frequency::set_abs);
	connect(&btn_option_rel, &QPushButton::clicked, this, &Frequency::set_rel);
	connect(header_view, &QHeaderView::sectionDoubleClicked, this, &Frequency::sort_by_colum);
}

void Frequency::sort_by_colum(int column)
{
	if (sort_asc == true) {
		table.sortByColumn(column, Qt::AscendingOrder);
		sort_asc = false;
	} else {
		table.sortByColumn(column, Qt::DescendingOrder);
		sort_asc = true;
	}
}

void Frequency::setArgs(const Args &args)
{
	ngram_len = args[arg1].toString().toInt();
	if (ngram_len < 0 || ngram_len > 14) {
		ngram_len = 1;
	}
	delimitter = args[arg2].toString();
	FilterInterface::setArgs(args);
}

void Frequency::settingsUi(QWidget *parent)
{
	settingsDialog dialog(parent);

	Args args = getArgs();
	dialog.addLineEdit(arg1, args[arg1]);
	dialog.addLineEdit(arg2, args[arg2]);

	if (dialog.exec() == QDialog::Accepted) {
		setArgs(dialog.args);
	}
}

QList<Page> Frequency::processPages(QList<Page> &pages)
{

	QString allInOne;
	for (Page &page : pages) {
		QString txt = page.plainText();
		txt = txt.replace("\n", delimitter);
		allInOne.append(txt).append(delimitter);
	}

	allInOne = allInOne.left(allInOne.size() - delimitter.length());
	process(allInOne);
	return pages;
}

void Frequency::reset()
{
	data.clear();
	total = 0;
}

QString Frequency::process(QString &text)
{

	QStringList chunks = text.split(delimitter);
	// check lenght of ngram inserted by user
	if ((ngram_len > 0) && (ngram_len < 16)) {
		QString ngram;
		ngram.reserve(ngram_len);

		for (int i = 0; i < (chunks.size() - (ngram_len - 1)); i++) {
			ngram.clear();
			for (int j = 0; j < ngram_len; j++) {
				ngram.append(chunks.at(i + j)).append(",");
			}
			ngram = ngram.left(ngram.length() - 1); // remove last ,
			if (ngram.isEmpty()) {
				continue;
			}
			if (data.find(ngram) != data.end()) {
				data[ngram]++;
			} else {
				data.insert(ngram, 1);
			}
			total++;
		}
	} else {
		qDebug() << "ngram lenght have to by in range <1, 15>";
	}

	return text;
}

void Frequency::resultUi(QWidget *parent)
{
	if (data.isEmpty()) {
		return;
	}

	QDialog *dialog = new QDialog(parent);
	QGridLayout main_layout;

	const qint32 columns_count = 3;

	// adding widgetss to layout
	main_layout.addWidget(&table, 0, 0, 1, 2);
	main_layout.addWidget(&le_file_path, 1, 0, 1, 1);
	main_layout.addWidget(&btn_open_file_browser, 1, 1, 1, 1);
	main_layout.addWidget(&btn_export_data, 2, 0, 1, 2);
	main_layout.addWidget(&btn_show_graph, 4, 0, 1, 2); //new button to layout
	main_layout.addWidget(&btn_option_abs, 3, 0, 1, 2);
	main_layout.addWidget(&btn_option_rel, 3, 1, 1, 2);

	dialog->setLayout(&main_layout);
	dialog->setWindowTitle("Histogram for the document");

	// setting rows, columns and labels for columns
	table.setRowCount(data.count() + 1);
	table.setColumnCount(columns_count);
	table.setHorizontalHeaderLabels(QStringList() << "ngram"
												  << "abs. frequency"
												  << "rel. frequency");
	// stretch the columns width to their content
	table.setHorizontalHeader(header_view);
	table.resizeColumnsToContents();

	// align the labels to left
	for (int i = 0; i < columns_count; i++) {
		table.horizontalHeaderItem(i)->setTextAlignment(Qt::AlignLeft);
	}

	// inserting data to table
	int i = 0;

	QList<quint32> vals = data.values();
	QSet<quint32> valsSet = vals.toSet();
	vals = valsSet.toList();
	qSort(vals);
	std::reverse(vals.begin(), vals.end());
	int totAbs = 0;
	float totRel = 0;
	foreach (quint32 val, vals) {
		QList<QString> keys = data.keys(val);
		foreach (QString key, keys) {
			QString ngram = key;
			ngram.push_front('[');
			ngram.push_back(']');

			QTableWidgetItem *itemAbs = new QTableWidgetItem;
			itemAbs->setData(Qt::DisplayRole, val);
			QTableWidgetItem *itemRel = new QTableWidgetItem;
			itemRel->setData(Qt::DisplayRole, (float)val / total);

			table.setItem(i, 0, new QTableWidgetItem(ngram));
			table.setItem(i, 1, itemAbs);
			table.setItem(i, 2, itemRel);
			totAbs += val;
			totRel += (float)val / total;
			i++;
		}
	}

	// summary
	QString total = "Total";
	QTableWidgetItem *itemAbs = new QTableWidgetItem;
	itemAbs->setData(Qt::DisplayRole, totAbs);
	QTableWidgetItem *itemRel = new QTableWidgetItem;
	itemRel->setData(Qt::DisplayRole, totRel);

	table.setItem(i, 0, new QTableWidgetItem(total));
	table.setItem(i, 1, itemAbs);
	table.setItem(i, 2, itemRel);

	dialog->show(); //exec to show
}

QString Frequency::export_histogram(void)
{
	QMap<QString, uint>::iterator map_it;

	QString filePath = le_file_path.text();

	if (filePath.isEmpty()) {
		filePath = QDir::currentPath();
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
	QString filename = Name + "_histogram_" + time + QString(".csv");

	QFile file(filePath + filename);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "Could not open " << file.fileName();
		return "Could not open file for writing, check your permissions";
	}

	qDebug() << "writing histogram to " << file.fileName();

	QTextStream stream(&file);
	stream.setCodec("UTF-8");

	stream << "character,abs. frequency, rel.frequency"
		   << "\n";
	int totAbs = 0;
	for (map_it = data.begin(); map_it != data.end(); map_it++) {
		stream << map_it.key() << "," << map_it.value() << "," << (float)map_it.value() / total << "\n";
		totAbs += map_it.value();
	}
	float totRel = (float)totAbs / total;
	stream << "total"
		   << "," << totAbs << "," << totRel;

	file.flush();
	file.close();
	return "Successfully exported to file.";
}

void Frequency::open_file_browser()
{
	QString dir = QFileDialog::getExistingDirectory();

	if (!dir.isEmpty()) {
		dir.append(QDir::separator());
		pallete.setColor(QPalette::Text, Qt::black);
		le_file_path.setPalette(pallete);
		le_file_path.setText(dir);
	}
}

void Frequency::show_graph() //show graph function(definition)
{
	QScrollArea *scrollArea = new QScrollArea();
	QWidget *widget = new Frequency::Graph(data, is_abs);
	scrollArea->setWidget(widget);
	scrollArea->resize(scrollArea->width(), widget->height() + 20);
	scrollArea->setMinimumHeight(widget->height() + 20);
	scrollArea->setMaximumHeight(widget->height() + 20);
	scrollArea->show();
	scrollArea->activateWindow();
}

void Frequency::set_abs()
{
	is_abs = true;
}

void Frequency::set_rel()
{
	is_abs = false;
}

Frequency::Graph::Graph(const QMap<QString, quint32> &data, bool is_abs) //graph window
	: barColor(Qt::green)
	, data(data)
	, max(0)
	, is_abs(is_abs)
	, max_offset(0)
	, max_width(0)

{
	this->resize(100 * data.size(), 580);
	if (!is_abs) {
		auto values = data.values();
		int total = std::accumulate(values.begin(), values.end(), 0);
		for (const auto &e : data.toStdMap()) {
			float rel = 100 * (float)e.second / (float)total;
			data_float.insert(e.first, rel);
		}
	}
	if (!data.empty()) {
		max = *std::max_element(data.begin(), data.end());
		max_float = *std::max_element(data_float.begin(), data_float.end());
	}
}

void Frequency::Graph::getMaxOffset(QPainter &painter)
{
	if (max_offset == 0) {
		for (quint32 i = 0; i < 10; ++i) {
			QString valueStr;
			if (is_abs) {
				int value = ((float)max / 10.0f) * (10.0f - (float)i);
				valueStr = QString::number(value);
			} else {
				int value = ((float)max_float / 10.0f) * (10.0f - (float)i);
				valueStr = QString::number(value) + " %";
			}
			QFontMetrics fm = painter.fontMetrics();
			int pixels = fm.width(valueStr);
			offsets[i] = pixels;
			if (pixels > max_offset) {
				max_offset = pixels;
			}
		}
	}
}

void Frequency::Graph::drawAxis(QPainter &painter) //graph axis
{
	painter.setBrush(QBrush(Qt::black));
	if (!data.empty()) {
		for (quint32 i = 0; i < 10; ++i) {
			int y = 10 + 570 / 10 * i;
			painter.drawLine(max_offset + 5, y, max_offset + 15, y);

			QString valueStr;
			if (is_abs) {
				int value = ((float)max / 10.0f) * (10.0f - (float)i);
				valueStr = QString::number(value);
			} else {
				int value = ((float)max_float / 10.0f) * (10.0f - (float)i);
				valueStr = QString::number(value) + " %";
			}
			QRect textRect(5 + (max_offset - offsets[i]), y, offsets[i], 20);
			painter.drawText(textRect, valueStr);
		}

		// Y axis
		painter.drawLine(max_offset + 10, 0, max_offset + 10, this->height() - 10);

		int x = max_offset + 10 + data.size() * (max_width + 10);
		// X axis
		painter.drawLine(10, this->height() - 20, x, this->height() - 20);
	}
}

void Frequency::Graph::getMaxWidth(QPainter &painter)
{
	if (max_width == 0) {
		for (const auto &e : data.keys()) {
			QFontMetrics fm = painter.fontMetrics();
			int pixels = fm.width(e);
			if (pixels > max_width) {
				max_width = pixels;
			}
		}
	}
}

void Frequency::Graph::drawBars(QPainter &painter) //graph bars
{
	painter.setBrush(QBrush(Qt::green));
	int barWidth = 10;
	int i = 0;

	for (const auto &val : data.toStdMap()) {
		if (i % 2) {
			painter.setBrush(QBrush(Qt::green));
		} else {
			painter.setBrush(QBrush(Qt::blue));
		}
		int height = ((float)val.second / (float)max) * ((float)this->height() - (float)30);
		int y = this->height() - 20 - height;

		int x = max_offset + 10 + i * (max_width + 10);

		QRect barRect(x + 10, y, barWidth, height);
		QRect textRect(x + 10, this->height() - 20, max_width, 15);

		painter.drawRect(barRect);
		painter.drawText(textRect, val.first);
		++i;
	}
}

void Frequency::Graph::paintEvent(QPaintEvent * /*event*/) //paint event
{
	QPainter painter(this);
	getMaxOffset(painter);
	getMaxWidth(painter);
	this->resize(max_offset + 10 + data.size() * (max_width + 10), 580);
	drawAxis(painter);
	drawBars(painter);
}
