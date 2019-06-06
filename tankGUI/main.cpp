#include "tankGUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	tankGUI w;
	w.show();

	return a.exec();
}
