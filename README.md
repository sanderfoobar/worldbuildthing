## World Build Thing

Asset management for the Splinter game engine.

### Server

todo

### Client

todo

### Notes

`sudo apt install -y libpng pkg-config cmake qt6-base-dev qt6-multimedia-dev qt6-websockets-dev qt6-httpserver-dev zip unzip`

#### systemd service

```
[Unit]
Description=Persistent asset service
After=network.target

[Service]
User=assets
Restart=on-failure
RestartSec=20
WorkingDirectory=/home/assets/server
ExecStart=bash /home/assets/server/bin/server
 
[Install]
WantedBy=multi-user.target
```