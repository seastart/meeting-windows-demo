#ifndef MEMBERDATAMODEL_H
#define MEMBERDATAMODEL_H

#include <QList>
#include "CJsonObject/JsonUtil.h"

class MemberPropsDataModel{
public:
    MemberPropsDataModel(){
        _audioState = false;
        _videoState = false;
    }
    bool audioState(){return _audioState;}
    void set_audioState(bool v){_audioState = v;}
    bool videoState(){return _videoState;}
    void set_videoState(bool v){_videoState = v;}
    void Load(QJsonObject qdoc){
        JsonUtil::GetValue(qdoc,"audioState",_audioState);
        JsonUtil::GetValue(qdoc,"videoState",_videoState);
    }
    QByteArray ToJson(){
        QJsonDocument doc;
        QJsonObject obj_props;
        QJsonObject obj;
        obj["audioState"] = _audioState;
        obj["videoState"] = _videoState;
        obj_props.insert("props",obj);
        doc.setObject(obj_props);
        return doc.toJson(QJsonDocument::JsonFormat::Compact);
    }
private:
    bool _audioState;
    bool _videoState;
};

class StreamDataModel
{
public:
    void Load(QJsonObject qdoc){
        if(!qdoc.isEmpty())
        {
            JsonUtil::GetValue(qdoc,"bitrate",_bitrate);
            JsonUtil::GetValue(qdoc,"codec_type",_codec_type);
            JsonUtil::GetValue(qdoc,"fps",_fps);
            JsonUtil::GetValue(qdoc,"height",_height);
            JsonUtil::GetValue(qdoc,"id",_id);
            JsonUtil::GetValue(qdoc,"type",_type);
            JsonUtil::GetValue(qdoc,"width",_width);
            JsonUtil::GetValue(qdoc,"media_type",_media_type);
        }
    }
    int bitrate(){return _bitrate;}
    void set_bitrate(int v){_bitrate = v;}
    int codec_type(){return _codec_type;}
    void set_codecType(int v){_codec_type = v;}
    int fps(){return _fps;}
    void set_fps(int v){_fps = v;}
    int height(){return _height;}
    void set_height(int v){_height = v;}
    int id(){return _id;}
    void set_id(int v){_id = v;}
    int type(){return _type;}
    void set_type(int v){_type = v;}
    int width(){return _width;}
    void set_width(int v){_width = v;}
    int media_type(){return _media_type;}
    void set_media_type(int v){_media_type = v;}
private:
    int _bitrate;
    int _codec_type;
    int _fps;
    int _height;
    int _id;
    int _type;
    int _width;
    int _media_type;
};

class MemberDataModel
{
public:
    MemberDataModel()
    {
        has_sid = false;
        has_uid = false;
        has_name = false;
        has_avatar = false;
        has_terminal_type = false;
        _terminal_type = 0;
        has_terminal_desc = false;
        has_updated_at = false;
        _updated_at = 0;
        has_link_id = false;
        has_room_id = false;
        has_role = false;
        _role = 0;
        has_session_key = false;
        has_streams = false;
        has_netid = false;
    }
    void Load(QJsonObject qdoc){
        if(!qdoc.isEmpty())
        {
            has_sid = JsonUtil::GetValue(qdoc,"sid",_sid);
            has_uid = JsonUtil::GetValue(qdoc,"uid",_uid);
            has_name = JsonUtil::GetValue(qdoc,"name",_name);
            has_terminal_type = JsonUtil::GetValue(qdoc,"terminal_type",_terminal_type);
            has_avatar = JsonUtil::GetValue(qdoc,"avatar",_avatar);
            has_terminal_desc = JsonUtil::GetValue(qdoc,"terminal_desc",_terminal_desc);
            has_updated_at = JsonUtil::GetValue(qdoc,"updated_at",_updated_at);
            has_link_id = JsonUtil::GetValue(qdoc,"link_id",_link_id);
            has_room_id = JsonUtil::GetValue(qdoc,"room_id",_room_id);
            has_role = JsonUtil::GetValue(qdoc,"role",_role);
            has_session_key = JsonUtil::GetValue(qdoc,"session_key",_session_key);
            has_netid = JsonUtil::GetValue(qdoc,"netid",_netid);
            QJsonObject props;
            has_props = JsonUtil::GetValue(qdoc,"props",props);
            if(has_props){
                _props.Load(props);
            }
            QJsonArray ss = qdoc.value("streams").toArray();
            has_streams = !ss.isEmpty();
            _streams.clear();
            for (QJsonArray::iterator it = ss.begin(); it != ss.end(); ++it)
            {
                QJsonObject stream = (*it).toObject();
                StreamDataModel s;
                s.Load(stream);
                _streams.append(s);
            }
        }
    }

    void MergeData(MemberDataModel& newdata){
        if(newdata.has_sid){set_sid(newdata._sid);}
        if(newdata.has_uid){set_uid(newdata._uid);}
        if(newdata.has_name){set_name(newdata._name);}
        if(newdata.has_avatar){set_avatar(newdata._avatar);}
        if(newdata.has_terminal_type){set_terminal_type(newdata._terminal_type);}
        if(newdata.has_terminal_desc){set_terminal_desc(newdata._terminal_desc);}
        if(newdata.has_updated_at){set_updated_at(newdata._updated_at);}
        if(newdata.has_link_id){set_link_id(newdata._link_id);}
        if(newdata.has_room_id){set_room_id(newdata._room_id);}
        if(newdata.has_role){set_role(newdata._role);}
        if(newdata.has_session_key){set_session_key(newdata._session_key);}
        if(newdata.has_netid){set_netid(newdata._netid);}
        if(newdata.has_props){set_props(newdata._props);}
        qDebug()<<__func__<<"has_streams"<<newdata._streams.size();
        set_streams(newdata._streams);

    }

    QString sid(){return _sid;}
    void set_sid(QString v){_sid = v;}
    QString uid(){return _uid;}
    void set_uid(QString v){_uid = v;}
    QString name(){return _name;}
    void set_name(QString v){_name = v;}
    QString avatar(){return _avatar;}
    void set_avatar(QString v){_avatar = v;}
    int terminal_type(){return _terminal_type;}
    void set_terminal_type(int v){_terminal_type = v;}
    QString terminal_desc(){return _terminal_desc;}
    void set_terminal_desc(QString v){_terminal_desc = v;}
    int updatedA_at(){return _updated_at;}
    void set_updated_at(int v){_updated_at = v;}
    QString link_id(){return _link_id;}
    void set_link_id(QString v){_link_id = v;}
    QString room_id(){return _room_id;}
    void set_room_id(QString v){_room_id = v;}
    int role(){return _role;}
    void set_role(int v){_role = v;}
    QString session_key(){return _session_key;}
    void set_session_key(QString v){_session_key = v;}
    QString netid(){return _netid;}
    void set_netid(QString v){_netid = v;}
    MemberPropsDataModel& props(){return _props;}
    void set_props(MemberPropsDataModel v){_props = v;}
    QList<StreamDataModel>& streams(){return _streams;}
    void set_streams(QList<StreamDataModel> v){_streams = v;}
private:
    bool has_sid;
    QString _sid;
    bool has_uid;
    QString _uid;
    bool has_name;
    QString _name;
    bool has_avatar;
    QString _avatar;
    bool has_terminal_type;
    int _terminal_type;
    bool has_terminal_desc;
    QString _terminal_desc;
    bool has_updated_at;
    int _updated_at;
    bool has_link_id;
    QString _link_id;
    bool has_room_id;
    QString _room_id;
    bool has_role;
    int _role;
    bool has_session_key;
    QString _session_key;
    bool has_netid;
    QString _netid;
    bool has_props;
    MemberPropsDataModel _props;
    bool has_streams;
    QList<StreamDataModel> _streams;

};

#endif // MEMBERDATAMODEL_H
