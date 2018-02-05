#include "mavlink_channels.h"
#include <gtest/gtest.h>

using namespace dronecore;

TEST(MavlinkChannels, MaxChannelsSanity)
{
    ASSERT_TRUE(MavlinkChannels::Instance().get_max_channels() < UINT8_MAX);
    ASSERT_TRUE(MavlinkChannels::Instance().get_max_channels() > 0);

}

TEST(MavlinkChannels, TryAll)
{
    // Checkout all first
    for (unsigned i = 0; i < UINT8_MAX; ++i) {
        uint8_t channel;
        if (i < MavlinkChannels::Instance().get_max_channels()) {
            ASSERT_TRUE(MavlinkChannels::Instance().checkout_free_channel(channel));
            ASSERT_EQ(i, channel);
        } else {
            ASSERT_FALSE(MavlinkChannels::Instance().checkout_free_channel(channel));
        }
    }

    // Give them all back, even the invalid ones
    for (unsigned i = 0; i < UINT8_MAX; ++i) {
        MavlinkChannels::Instance().checkin_used_channel(i);
    }
}

TEST(MavlinkChannels, ReuseChannels)
{
    // Checkout 0,1,2
    uint8_t channels[3];
    ASSERT_TRUE(MavlinkChannels::Instance().checkout_free_channel(channels[0]));
    ASSERT_TRUE(MavlinkChannels::Instance().checkout_free_channel(channels[1]));
    ASSERT_TRUE(MavlinkChannels::Instance().checkout_free_channel(channels[2]));

    // Give back 1.
    MavlinkChannels::Instance().checkin_used_channel(channels[1]);

    // And ask for 1 once again.
    uint8_t new_channel;
    ASSERT_TRUE(MavlinkChannels::Instance().checkout_free_channel(new_channel));
    ASSERT_EQ(new_channel, 1);

    // And make sure it continues at 3.
    ASSERT_TRUE(MavlinkChannels::Instance().checkout_free_channel(new_channel));
    ASSERT_EQ(new_channel, 3);
}
