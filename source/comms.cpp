#include "comms.h"
#include <cstring>


void PlayerData::fill(const PlayerMetaPacket& p)
{
    color = glm::vec4{
        p.cross_r/255.0f,
        p.cross_g/255.0f,
        p.cross_b/255.0f,
        p.cross_a/255.0f
    };
    memcpy(username, p.username, MAX_NAME_LEN);
}
void PlayerData::fill(const ServerPlayerPacket& p)
{
    looking_at_x = p.looking_at_x;
    looking_at_y = p.looking_at_y;

    const auto oldPos = position;
    position[0] = ENET_NET_TO_HOST_32(p.x) / POS_SCALE;
    position[2] = ENET_NET_TO_HOST_32(p.y) / POS_SCALE;
    movedDistance = glm::distance(position, oldPos);

    const auto yaw_bytes = ENET_NET_TO_HOST_16(p.yaw);
    int16_t yaw_int = 0;
    memcpy(&yaw_int, &yaw_bytes, sizeof(int16_t));
    yaw = yaw_int;

    const auto pitch_bytes = ENET_NET_TO_HOST_16(p.pitch);
    int16_t pitch_int = 0;
    memcpy(&pitch_int, &pitch_bytes, sizeof(int16_t));
    pitch = pitch_int;
}

PlayerMetaPacket PlayerData::fill_meta() const
{
    PlayerMetaPacket out;
    out.cross_r = 255 * color[0];
    out.cross_g = 255 * color[1];
    out.cross_b = 255 * color[2];
    out.cross_a = 255 * color[3];
    memcpy(out.username, username, sizeof(out.username));
    return out;
}
ServerPlayerPacket PlayerData::fill_info() const
{
    ServerPlayerPacket out;
    out.x = ENET_HOST_TO_NET_32(enet_uint32(position[0] * POS_SCALE));
    out.y = ENET_HOST_TO_NET_32(enet_uint32(position[2] * POS_SCALE));

    enet_uint16 yaw_bytes = 0;
    memcpy(&yaw_bytes, &yaw, sizeof(int16_t));
    out.yaw = ENET_HOST_TO_NET_16(yaw_bytes);

    enet_uint16 pitch_bytes = 0;
    memcpy(&pitch_bytes, &pitch, sizeof(int16_t));
    out.pitch = ENET_HOST_TO_NET_16(pitch_bytes);

    out.looking_at_x = looking_at_x;
    out.looking_at_y = looking_at_y;
    return out;
}
