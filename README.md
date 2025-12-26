## World Build Thing

3d editor

```bash
cd external/qcoro
cmake -Bbuild -DCMAKE_PREFIX_PATH=/home/dsc/Qt691_full_no_debug/6.9.3/gcc_64 -DUSE_QT_VERSION=6 -DCORO_BUILD_EXAMPLES=OFF -DQCORO_BUILD_TESTING=OFF .
sudo make -Cbuild -j12

cmake -Bbuild -DCMAKE_PREFIX_PATH=/home/dsc/Qt691_full_no_debug/6.9.3/gcc_64 .
make -Cbuild -j12
```

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