#pragma once
#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QColor>

class LoadingWidget : public QWidget
{
	Q_OBJECT
		Q_PROPERTY(int delay READ animationDelay WRITE setAnimationDelay)
		Q_PROPERTY(bool displayedWhenStopped READ isDisplayedWhenStopped WRITE setDisplayedWhenStopped)
		Q_PROPERTY(QColor color READ color WRITE setColor)
public:
	LoadingWidget(QWidget* parent = 0);

	int animationDelay() const { return delay_; }

	bool isAnimated() const;

	bool isDisplayedWhenStopped() const;

	const QColor& color() const { return color_; }

	virtual QSize sizeHint() const;

	void setBackground(const QString& _icon) {
		currentPix_ = QPixmap(_icon);
	}
signals:
	void Finished(void);
public slots:
	void startAnimation();

	void stopAnimation();

	void setAnimationDelay(int delay);

	void setDisplayedWhenStopped(bool state);

	void setColor(const QColor& color);
	void onProgress(short _progress) { progress_ = _progress; }
protected:
	virtual void timerEvent(QTimerEvent* event);
	virtual void paintEvent(QPaintEvent* event);
private:
	unsigned int angle_;
	int timerId_;
	int delay_;
	bool displayedWhenStopped_;
	QColor color_;
	short progress_;
	QPixmap currentPix_;
};
