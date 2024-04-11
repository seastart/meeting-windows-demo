#include "SetImageClass.h"
#include "Global/GlobalDataClass.h"
//#define HASNETWORKDOWNLOAD
#include "Network/HttpNetwork.h"
#include <QPainterPath>
#include <QFile>
#include <QPainter>
//设置头像的图片 按钮控件，
void SetImageClass::SetPortraitImage(QPushButton* btn, QString ID, QString url, QString defimage)
{
    if (url.size() != 0)
    {
        QString portraitPath = GlobalDataClass::Get()->PortraitPath() + ID + ".png";
        if (!ID.isEmpty() && QFile::exists(portraitPath))
        {
            QPixmap currentPicture;
            if (!LoadPixmapType(portraitPath, currentPicture))
            {
                LoadDefPixmap(btn, defimage);
                return;
            }
            QPixmap qpRound = PixmapToRound(btn, currentPicture);
            if (!qpRound.isNull())
            {
                LoadPixmap(btn, &qpRound);
            }
            else
            {
                LoadDefPixmap(btn, defimage);
            }
        }
        else
        {
            HttpNetwork::OnDownload(url.toLocal8Bit(), [btn, portraitPath, defimage, ID](QByteArray& byte) -> void {
                QPixmap currentPicture;
                if (currentPicture.loadFromData(byte))
                {
                    if (!ID.isEmpty())
                    {
                        currentPicture.save(portraitPath);
                    }
                    QPixmap qpRound = PixmapToRound(btn, currentPicture);
                    if (!qpRound.isNull())
                    {
                        LoadPixmap(btn, &qpRound);
                    }
                    else
                    {
                        LoadDefPixmap(btn, defimage);
                    }
                }
                else
                {
                    if (LoadDefPixmap(btn, defimage))
                    {
                        if (!ID.isEmpty())
                        {
                            currentPicture.save(portraitPath);
                        }
                    }
                }
                });
        }
    }
    else
    {
        LoadDefPixmap(btn, defimage);
    }
}

//设置头像的图片 文本控件，
void SetImageClass::SetPortraitImage(QLabel* lbl, QString ID, QString url, QString defimage)
{
	if (url.size() != 0)
	{
        QString portraitPath = GlobalDataClass::Get()->PortraitPath() + ID + ".png";
		if (!ID.isEmpty() && QFile::exists(portraitPath))
		{
			QPixmap currentPicture;
			if (!LoadPixmapType(portraitPath, currentPicture))
			{
                LoadDefPixmap(lbl, defimage);
				return;
			}
			QPixmap qpRound = PixmapToRound(lbl, currentPicture);
			if (!qpRound.isNull())
			{
				LoadPixmap(lbl, &qpRound);
			}
			else
			{
				LoadDefPixmap(lbl, defimage);
			}
		}
		else
		{
            HttpNetwork::OnDownload(url.toLocal8Bit(), [lbl, portraitPath, defimage, ID](QByteArray& byte) -> void {
				QPixmap currentPicture;

				if (currentPicture.loadFromData(byte))
				{
					if (!ID.isEmpty())
					{
						currentPicture.save(portraitPath);
					}
					QPixmap qpRound = PixmapToRound(lbl, currentPicture);
					if (!qpRound.isNull())
					{
						LoadPixmap(lbl, &qpRound);
					}
					else
					{
						LoadDefPixmap(lbl, defimage);
					}
				}
				else
                {
                    if (LoadDefPixmap(lbl, defimage))
					{
						if (!ID.isEmpty())
						{
							currentPicture.save(portraitPath);
						}
					}
				}
				});
		}
	}
    else
	{
        LoadDefPixmap(lbl, defimage);
	}
}


void SetImageClass::StopPortraitImageDownLoad(QString url)
{
	if (!url.isEmpty())
	{
		HttpNetwork::ClearUrlPost(url.toLocal8Bit());
	}
}

bool SetImageClass::LoadDefPixmap(QLabel* lbl, QString pix)
{
    QPixmap def = PixmapToRound(lbl,pix);
	if (!def.isNull())
	{
		return LoadDefPixmap(lbl, &def);

	}
	return false;
}

bool SetImageClass::LoadDefPixmap(QLabel* lbl, QPixmap* defimage)
{
	if (defimage != nullptr)
	{
		bool ret = LoadPixmap(lbl, defimage);
		return ret;
	}
	return false;
}
bool SetImageClass::LoadDefPixmap(QPushButton* btn, QString pix)
{
    QPixmap def = PixmapToRound(btn,pix);
	if (!def.isNull())
	{
		return LoadDefPixmap(btn, &def);
	}
	return false;
}
bool SetImageClass::LoadDefPixmap(QPushButton* btn, QPixmap* defimage)
{
	if (defimage != nullptr)
	{
		LoadPixmap(btn, defimage);
		return true;
	}
	return false;
}

bool SetImageClass::LoadPixmap(QLabel* lbl, QPixmap* pix)
{
	if (lbl)
	{
		lbl->setPixmap(*pix);
		return true;
	}
	else
	{
		return false;
	}
}

bool SetImageClass::LoadPixmap(QPushButton* btn, QPixmap* pix)
{
	btn->setIcon(*pix);
	return true;
}

bool SetImageClass::LoadPixmapType(QString path, QPixmap& pixmap)
{
	if (!pixmap.load(path, "png"))
	{
		if (!pixmap.load(path, "jpg"))
		{
			qDebug() << __func__ << path << "portrait error.";
			return false;
		}
	}
	return true;
}

QPixmap SetImageClass::PixmapToRound(QWidget* wid, QPixmap pix)
{
	QPixmap pixmap = pix.scaled(wid->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	int width = wid->size().width();
	int height = wid->size().height();

	QPixmap image(width, height);
	image.fill(Qt::transparent);

	QPainterPath painterPath;
	painterPath.addEllipse(0, 0, width, height);

	QPainter painter(&image);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	painter.setClipPath(painterPath);
	painter.drawPixmap(0, 0, width, height, pixmap);
	//绘制到label
	return image;
}
