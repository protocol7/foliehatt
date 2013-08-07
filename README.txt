Wifi thermin

This was our hack at wowhack2 where we wanted to build a Wifi theremin running
on Raspberry Pi. Basically, we wanted to make a musical instrument similar to a
theremin by regulating wifi signal strengths. In practice it turned out quite a
bit harder then anticipated, mostly due to our inability to, within reasonable
latency, change the signal level. Radio waves, it seems, have their ways to
sneak out of our best contraptions and the wifi signal modulation is slow.

We used a Raspberry Pi with latest Raspbian with two Realtek RTL8192U USB wifi
adapters. Firmware for these was already in Raspbian so they worked out of the
box.

Scanning vs associated signal levels: we initially wanted to use scanning in
order to have a single wifi adapter be able to provide signal levels for
multiple SSIDs but the driver for the Realtek adaopters does not support
scanning a specific SSID (iwlist <interface> scan essid <SSID>) meaning that we
would have to scan the full list of SSIDs, which is slow. Thus we instead opted
for two wifi adapters associated with two wifis. Even so, the Realtek driver
only provides signal level updates every second, so that's our minimal latency.

For WPA authentication wpa_supplicant was used with network configuration like
this:

network={
ssid="SSID-GOES-HERE"
proto=RSN
key_mgmt=WPA-PSK
pairwise=CCMP TKIP
group=CCMP TKIP
psk="WIFI-PASSWORD-GOES-HERE"
}


Get two wlan interfaces running
* Add additional interface to /etc/network/interfaces
* Add separete wpa_supplicant.conf for the second interface
* Kill any running wpa_supplicant or wpa_cli
* sudo /sbin/wpa_supplicant -s -B -P /var/run/wpa_supplicant.wlan0.pid -i wlan0 -W -D nl80211,wext -c /etc/wpa_supplicant/wpa_supplicant.conf
* sudo /sbin/wpa_supplicant -s -B -P /var/run/wpa_supplicant.wlan1.pid -i wlan1 -W -D nl80211,wext -c /etc/wpa_supplicant/wpa_supplicant2.conf
* sudo /sbin/wpa_cli -B -P /var/run/wpa_action.wlan0.pid -i wlan0 -p /var/run/wpa_supplicant -a /sbin/wpa_action
* sudo /sbin/wpa_cli -B -P /var/run/wpa_action.wlan1.pid -i wlan1 -p /var/run/wpa_supplicant2 -a /sbin/wpa_action

To restart the wlans
* sudo wpa_action wlan0 stop ; sudo wpa_action wlan0 reload ; sudo ifup -v wlan0
* sudo wpa_action wlan1 stop ; sudo wpa_action wlan1 reload ; sudo ifup -v wlan1
