#include "LoadingWidget.h"

LoadingWidget::LoadingWidget(QWidget* parent)
	: QWidget(parent),
	angle_(0),
	timerId_(-1),
	delay_(100),
	displayedWhenStopped_(false),
	progress_(0),
	color_(Qt::green) {
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setFocusPolicy(Qt::NoFocus);
}

bool LoadingWidget::isAnimated() const {
	return (timerId_ != -1);
}

void LoadingWidget::setDisplayedWhenStopped(bool state) {
	displayedWhenStopped_ = state;

	update();
}

bool LoadingWidget::isDisplayedWhenStopped() const {
	return displayedWhenStopped_;
}

void LoadingWidget::startAnimation() {
	angle_ = 0;

	if (timerId_ == -1) {
		timerId_ = startTimer(delay_);
	}
}

void LoadingWidget::stopAnimation() {
	if (timerId_ != -1) {
		killTimer(timerId_);
	}

	timerId_ = -1;

	update();
}

void LoadingWidget::setAnimationDelay(int delay) {
	if (timerId_ != -1) {
		killTimer(timerId_);
	}

	delay_ = delay;

	if (timerId_ != -1) {
		timerId_ = startTimer(delay_);
	}
}

void LoadingWidget::setColor(const QColor& color) {
	color_ = color;

	update();
}

QSize LoadingWidget::sizeHint() const {
	return QSize(50, 50);
}


void LoadingWidget::timerEvent(QTimerEvent* /*event*/) {
	angle_ = (angle_ + 30) % 360;

	update();
}

void LoadingWidget::paintEvent(QPaintEvent* /*event*/) {
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);
	if (!displayedWhenStopped_ && !isAnimated()) {
		p.drawPixmap(rect(), currentPix_);
		return;
	}

	int width = qMin(this->width(), this->height());


	int outerRadius = (width - 1) >> 1;
	int innerRadius = ((width - 1) >> 1) * 0.5;

	int capsuleHeight = outerRadius - innerRadius;
	int capsuleWidth = (width > 32) ? capsuleHeight * .23 : capsuleHeight * .35;
	int capsuleRadius = capsuleWidth >> 1;

	/* 撰写进度 */
	//if (progress_ > 0 && progress_ < 100) {
	//    p.setPen(color_);
	//    p.drawText(rect(), Qt::AlignCenter, QString("%1%").arg(progress_));
	//}
	//else if (progress_ == 100) {
	//    stopAnimation();
	//}

	for (int i = 0; i < 12; ++i) {
		QColor color = color_;
		color.setAlphaF(1.0f - (i / 12.0f));
		p.setPen(Qt::NoPen);
		p.setBrush(color);
		p.save();
		p.translate(rect().center());
		p.rotate(angle_ - i * 30.0f);
		p.drawRoundedRect(((-capsuleWidth) >> 1), -(innerRadius + capsuleHeight), capsuleWidth, capsuleHeight, capsuleRadius, capsuleRadius);
		p.restore();
	}
}
