#ifndef ROOMDATAMODEL_H
#define ROOMDATAMODEL_H

#include "../CJsonObject/JsonUtil.h"


#define SHARESTARE_NONE "none"
#define SHARESTARE_DESKTOP "desktop"
#define SHARESTARE_WHITEBOARD "whiteboard"
class RoomPropsData{
public:
    RoomPropsData(){
        _shareTrack = 2;
    }
    QString uid(){return _shareUid;}
    void set_uid(QString v){ _shareUid = v;}


    QString shareState(){return _shareState;}
    void set_shareState(QString v){_shareState = v;}

    int shareTrack() { return _shareTrack; }
    void set_shareTrack(int v) { _shareTrack = v; }

    void Load(QJsonObject qdoc){
        JsonUtil::GetValue(qdoc,"shareState",_shareState);
        JsonUtil::GetValue(qdoc,"shareUid",_shareUid);
        JsonUtil::GetValue(qdoc, "shareTrack", _shareTrack);
    }
    QByteArray toJson() {
        QJsonDocument doc;
        QJsonObject obj;
        QJsonObject obj_props;
        obj["shareState"] = _shareState;
        obj["shareUid"] = _shareUid;
        obj["shareTrack"] = _shareTrack;
        obj_props.insert("props", obj);
        doc.setObject(obj_props);
        return doc.toJson(QJsonDocument::Compact);
    }
private:
    QString _shareUid;
    QString _shareState;
    int _shareTrack = 2;
};

class RoomDataModel{
public:
    RoomDataModel(){
        _updated_at = 0;
    }
    QString link_id(){return _link_id;}
    void set_link_id(QString v){_link_id = v;}

    QString room_id(){return _room_id;}
    void set_room_id(QString v){_room_id = v;}


    int updated_at(){return _updated_at;}
    void set_updated_at(int v){_updated_at = v;}


    RoomPropsData& props(){return _props;}
    void set_props(RoomPropsData v){_props = v;}

    void Load(QJsonObject qdoc){
        if(!qdoc.empty()){
            JsonUtil::GetValue(qdoc,"link_id",_link_id);
            QJsonObject props;
            JsonUtil::GetValue(qdoc,"props",props);
            _props.Load(props);
            JsonUtil::GetValue(qdoc,"room_id",_room_id);
            JsonUtil::GetValue(qdoc,"updated_at",_updated_at);
        }
    }
private:
    QString _link_id;
    RoomPropsData _props;
    QString _room_id;
    int _updated_at;
};
#endif // ROOMDATAMODEL_H
