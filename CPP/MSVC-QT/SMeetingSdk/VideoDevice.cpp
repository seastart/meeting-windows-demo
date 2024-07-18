
#include "VideoDevice.h"

void Resolution::ResolutionLoad(QJsonObject obj)
{
    if (!obj.isEmpty())
    {
        JsonUtil::GetValue(obj, "width", m_iWidth);
        JsonUtil::GetValue(obj, "height", m_iHeight);
        JsonUtil::GetValue(obj, "type", m_type);
    }
}

bool VideoDevice::Load(QJsonObject obj)
{
    if (!obj.isEmpty())
    {
        DeviceBase::Load(obj);
        QJsonArray ss = obj.value("resolutions").toArray();
        for (QJsonArray::iterator it = ss.begin(); it != ss.end(); ++it)
        {
            QJsonObject stream = (*it).toObject();
            Resolution ss;
            ss.ResolutionLoad(stream);
            //if (ss.GetWidth() * 9 != ss.GetHeight() * 16)
            //{
            //	continue;
            //}
            resolutions.append(ss);
        }

        return true;
    }
    return false;
}

bool ScreenDevice::Load(QJsonObject obj)
{
    if (!obj.isEmpty())
    {
        DeviceBase::Load(obj);
        int x,y,width,height;
        x=y=width=height = 0;
        JsonUtil::GetValue(obj,"x",x);
        JsonUtil::GetValue(obj,"y",y);
        JsonUtil::GetValue(obj,"width",width);
        JsonUtil::GetValue(obj,"height",height);
        JsonUtil::GetValue(obj,"hwnd",_hwnd);
        _rect = QRect(x,y,width,height);
        return true;
    }
    return false;
}



bool DeviceBase::Load(QJsonObject obj)
{
    if (!obj.isEmpty())
    {
        JsonUtil::GetValue(obj, "name", name);
        QString type = "";
        JsonUtil::GetValue(obj, "devId", devId);
        JsonUtil::GetValue(obj, "type", type);
        if (type == "default")
        {
            def = true;
        }
        return true;
    }
    return false;
}

bool VideoDeviceList::DevContains(QString devName, VideoDevice& res)
{
    for (auto d : devices)
    {
        if (d.GetName() == devName)
        {
            res = d;
            return true;
        }
    }
    return false;
}

void VideoDeviceList::Load(QJsonObject obj)
{
    int count = 0;
    JsonUtil::GetValue(obj, "count", count);

    QJsonArray ss = obj.value("equip_list").toArray();
    for (QJsonArray::iterator it = ss.begin(); it != ss.end(); ++it)
    {
        QJsonObject eq = (*it).toObject();
        VideoDevice vd;
        bool ret = vd.Load(eq);

        for (auto d : devices)
        {
            if (d.GetResolutionCount() > 0)
            {
                hasDevice = true;
            }
        }

        devices.append(vd);
    }
}

void ScreenDeviceList::Load(QJsonObject obj)
{
    int count = 0;
    JsonUtil::GetValue(obj, "count", count);

    QJsonArray ss = obj.value("equip_list").toArray();
    for (QJsonArray::iterator it = ss.begin(); it != ss.end(); ++it)
    {
        hasDevice = true;
        QJsonObject eq = (*it).toObject();
        ScreenDevice vd;
        vd.Load(eq);
        devices.append(vd);
    }
}

//DeviceBase DeviceList::DevContains(QString devName, bool& findFinish);
bool DeviceList::DevContains(QString devName, DeviceBase& Res)
{
    for (auto d : devices)
    {
        if (d.GetName() == devName)
        {
            Res = d;
            return true;
        }
    }
    return false;

}

QString DeviceList::GetDevault()
{
    for (auto d : devices)
    {
        if (d.GetDef())
        {
            return d.GetName();
        }
    }
    qDebug() << __func__ << "not find default";
    return "";
}

void DeviceList::Load(QJsonObject obj,QString name)
{
    if (!obj.isEmpty())
    {
        int count = 0;
        JsonUtil::GetValue(obj, "count", count);

        QJsonArray ss = obj.value("equip_list").toArray();
        for (QJsonArray::iterator it = ss.begin(); it != ss.end(); ++it)
        {
            QJsonObject eq = (*it).toObject();
            DeviceBase db;
            if (db.Load(eq))
            {
                devices.append(db);
            }
        }
    }
}
