apt-get update
apt-get upgarde
apt install -y build-essential
apt install -y curl
apt-get install -y libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-bad1.0-dev gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav gstreamer1.0-doc gstreamer1.0-tools gstreamer1.0-x gstreamer1.0-alsa gstreamer1.0-gl gstreamer1.0-gtk3 gstreamer1.0-qt5 gstreamer1.0-pulseaudio
apt install -y  g++ cmake pkg-config
apt install -y ffmpeg
apt install -y gedit
curl -o sample.mp4 http://commondatastorage.googleapis.com/gtv-videos-bucket/sample/BigBuckBunny.mp4
ffmpeg -i sample.mp4 -c copy -hls_time 10 -hls_list_size 0 -hls_segment_size 500000 -f hls master.m3u8
