#include <string>
#include "protocol.hpp"

std::string to_string(MsgProtocole msg) {
    switch (msg) {
        case MsgProtocole::AUTH_LOGIN_REQ: return "AUTH_LOGIN_REQ";
        case MsgProtocole::AUTH_REGISTER_REQ: return "AUTH_REGISTER_REQ";
        case MsgProtocole::AUTH_RESULT: return "AUTH_RESULT";
        case MsgProtocole::LOB_INFO_USER_REP: return "LOB_INFO_USER_REP";
        case MsgProtocole::LOB_CREATE_PROJECT_REQ: return "LOB_CREATE_PROJECT_REQ";
        case MsgProtocole::LOB_PROJECT_LIST_REQ: return "LOB_PROJECT_LIST_REQ";
        case MsgProtocole::LOB_PROJECT_LIST_REP: return "LOB_PROJECT_LIST_REP";
        case MsgProtocole::LOB_SHARE_PROJECT_REQ: return "LOB_SHARE_PROJECT_REQ";
        case MsgProtocole::LOB_SHARE_PROJECT_REP: return "LOB_SHARE_PROJECT_REP";
        case MsgProtocole::LOB_JOIN_PROJECT_REQ: return "LOB_JOIN_PROJECT_REQ";
        case MsgProtocole::LOB_IMPORT_PROJECT_REQ: return "LOB_IMPORT_PROJECT_REQ";
        case MsgProtocole::LOB_EXPORT_PNG_PROJECT_REQ: return "LOB_EXPORT_PNG_PROJECT_REQ";
        case MsgProtocole::LOB_EXPORT_JSON_PROJECT_REP: return "LOB_EXPORT_JSON_PROJECT_REP";
        case MsgProtocole::LOB_GET_MY_PROJECTS_DATA_REQ: return "LOB_GET_MY_PROJECTS_DATA_REQ";
        case MsgProtocole::LOB_GET_MY_PROJECTS_DATA_REP: return "LOB_GET_MY_PROJECTS_DATA_REP";
        case MsgProtocole::MAP_CREATE_LAYER_REQ: return "MAP_CREATE_LAYER_REQ";
        case MsgProtocole::MAP_CREATE_LAYER_REP: return "MAP_CREATE_LAYER_REP";
        case MsgProtocole::MAP_RENAME_LAYER_REQ: return "MAP_RENAME_LAYER_REQ";
        case MsgProtocole::MAP_RENAME_LAYER_REP: return "MAP_RENAME_LAYER_REP";
        case MsgProtocole::MAP_REMOVE_LAYER_REQ: return "MAP_REMOVE_LAYER_REQ";
        case MsgProtocole::MAP_REMOVE_LAYER_REP: return "MAP_REMOVE_LAYER_REP";
        case MsgProtocole::MAP_LOCK_LAYER_REQ: return "MAP_LOCK_LAYER_REQ";
        case MsgProtocole::MAP_LOCK_LAYER_REP: return "MAP_LOCK_LAYER_REP";
        case MsgProtocole::MAP_MOV_LAYER_REQ: return "MAP_MOV_LAYER_REQ";
        case MsgProtocole::MAP_MOV_LAYER_REP: return "MAP_MOV_LAYER_REP";
        case MsgProtocole::MAP_ORGANIZE_LAYER_REQ: return "MAP_ORGANIZE_LAYER_REQ";
        case MsgProtocole::MAP_ORGANIZE_LAYER_REP: return "MAP_ORGANIZE_LAYER_REP";
        case MsgProtocole::MAP_MASK_LAYER_REQ: return "MAP_MASK_LAYER_REQ";
        case MsgProtocole::MAP_MASK_LAYER_REP: return "MAP_MASK_LAYER_REP";
        case MsgProtocole::MAP_IMPORT_SPRITE_REQ: return "MAP_IMPORT_SPRITE_REQ";
        case MsgProtocole::MAP_IMPORT_SPRITE_REP: return "MAP_IMPORT_SPRITE_REP";
        case MsgProtocole::MAP_PUT_SPRITE_REQ: return "MAP_PUT_SPRITE_REQ";
        case MsgProtocole::MAP_PUT_SPRITE_REP: return "MAP_PUT_SPRITE_REP";
        case MsgProtocole::MAP_REMOVE_SPRITE_REQ: return "MAP_REMOVE_SPRITE_REQ";
        case MsgProtocole::MAP_REMOVE_SPRITE_REP: return "MAP_REMOVE_SPRITE_REP";
        case MsgProtocole::MAP_MOV_SPRITE_REQ: return "MAP_MOV_SPRITE_REQ";
        case MsgProtocole::MAP_MOV_SPRITE_REP: return "MAP_MOV_SPRITE_REP";
        case MsgProtocole::MAP_ROTATE_SPRITE_REQ: return "MAP_ROTATE_SPRITE_REQ";
        case MsgProtocole::MAP_ROTATE_SPRITE_REP: return "MAP_ROTATE_SPRITE_REP";
        case MsgProtocole::MAP_RESIZE_SPRITE_REQ: return "MAP_RESIZE_SPRITE_REQ";
        case MsgProtocole::MAP_RESIZE_SPRITE_REP: return "MAP_RESIZE_SPRITE_REP";
        case MsgProtocole::MAP_PUT_PIXEL_REQ: return "MAP_PUT_PIXEL_REQ";
        case MsgProtocole::MAP_PUT_PIXEL_REP: return "MAP_PUT_PIXEL_REP";
        case MsgProtocole::CHAT_MESSAGE_REQ: return "CHAT_MESSAGE_REQ";
        case MsgProtocole::CHAT_MESSAGE_REP: return "CHAT_MESSAGE_REP";
        case MsgProtocole::CHAT_CONNECT_USER_REP: return "CHAT_CONNECT_USER_REP";
        case MsgProtocole::CHAT_DISCONNECT_USER_REP: return "CHAT_DISCONNECT_USER_REP";
        case MsgProtocole::CHAT_LOCK_LAYER_REP: return "CHAT_LOCK_LAYER_REP";
    }

    return "UNKNOWN";
}