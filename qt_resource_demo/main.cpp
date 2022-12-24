/**
 * The "data" structure is roughly a series of "entry headers" followed by data,
 * which takes the form of the structure below.
 *
 * struct QtResourceDataEntryHeader {
 * 	uint16_t unknown;
 * 	uint16_t length;
 * 	// uint8_t data[length];
 * };
 *
 * The name structure has a similar format, in which each name has a header and
 * is followed by its string content, encoded in UTF-16.
 *
 * struct QtNameDataEntryHeader {
 * 	uint16_t length;
 * 	uint32_t unknown;
 * 	// uint16_t data[length];
 * };
 *
 * NOTE: All integer values are little endian.
 */

#include <QtCore/QFile>
#include <QtGui/QFontDatabase>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>

class MainWindow : public QDialog {
public:
	MainWindow(QWidget* parent = nullptr)
	    : QDialog(parent)
	{
		setWindowTitle("Resource Demo");
		setFixedSize(640, 384);

		QFile loremFile(":/assets/lorem.txt");
		loremFile.open(QFile::ReadOnly | QFile::Text);

		auto browser = new QTextBrowser;
		browser->setPlainText(loremFile.readAll());
		browser->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));

		auto layout = new QVBoxLayout(this);
		layout->addWidget(browser);
	}
};

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	auto mainWindow = new MainWindow;
	mainWindow->show();

	app.exec();
}
