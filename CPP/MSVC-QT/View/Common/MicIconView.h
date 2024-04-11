#pragma once
#include <QWidget>
#include <QPaintEvent>
#include <QTimer>
class MicIconView : public QWidget
{
	Q_OBJECT
public:
	MicIconView(QWidget* parent = nullptr);
	~MicIconView();

	void AddDb(int db);
	void OpenDev(int v) {
		openclose = v;
		update();
	}
	void SetBackColor(QColor c) {
		_backColor = c;
		update();
	}
protected:
	void paintEvent(QPaintEvent* event);
private:
	QColor _backColor;
	int openclose = 0;
	double h = 0;
	int index_lss = 0;
	int index_lss_10 = 0;
	double index_lss_10_1 = 0;
};

