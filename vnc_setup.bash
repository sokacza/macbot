sudo apt install lxde
sudo apt install tigervnc-standalone-server tigervnc-xorg-extension
touch ~/.vnc/xstartup
echo -e "unset SESSION_MANAGER\nunset DBUS_SESSION_BUS_ADDRESS\nstartlxde &" > ~/.vnc/xstartup
echo "PLEASE SET YOUR VNC ACCESS CREDENTIALS"
gsettings set org.gnome.Vino authentication-methods "['vnc']"
gsettings set org.gnome.Vino vnc-password $(echo -n '9055259140'|base64)
vncserver
mkdir -p ~/.config/autostart
touch ~/.config/autostart/vnc2.bash
echo -e "#! /bin/sh \nnvncserver :2 -depth 24 -geometry 1920x1080" > ~/.config/autostart/vnc2.bash
chmod +x ~/.config/autostart/vnc2.bash
sudo wget --output-document=/usr/share/backgrounds/MacBotWallpaper.png https://github.com/sokacza/macbot/raw/main/docs/Labs/Wallpapers/MARCWallpaper.png
sudo reboot

