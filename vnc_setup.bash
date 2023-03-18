sudo apt install lxde
sudo apt install tigervnc-standalone-server tigervnc-xorg-extension
touch ~/.vnc/xstartup
echo -e "unset SESSION_MANAGER\nunset DBUS_SESSION_BUS_ADDRESS\nstartlxde &" > ~/.vnc/xstartup
echo "PLEASE SET YOUR VNC ACCESS CREDENTIALS"
gsettings set org.gnome.Vino authentication-methods "['vnc']"
gsettings set org.gnome.Vino vnc-password $(echo -n '9055259140'|base64)
vncserver

