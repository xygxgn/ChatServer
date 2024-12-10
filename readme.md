# Chat server

* boost
  * `sudo apt-get install libboost-all-dev -y`

* muduo
  * `git clone https://github.com/chenshuo/muduo.git`
  * `./build.sh`
  * `./build.sh install`
  * `mv ../build ./`
  * `sudo cp -r ./build/release-install-cpp11/include/* /usr/local/include/`
  * `sudo cp -r ./build/release-install-cpp11/lib/* /usr/local/lib/`

* mysql
  * `sudo apt-get install mysql-server -y`
  * `sudo apt-get install libmysqlclient-dev -y`
  * `sudo service mysql start`

* redis
  * `sudo apt-get install redis-server -y`
  * `sudo systemctl status redis-server`
  * `sudo systemctl restart redis-server`
  * `git clone git clone https://github.com/redis/hiredis.git`
  * `cd hiredis`
  * `make`
  * `sudo make install`
  * `sudo netstat -tanp`

* nginx
  * `cd ~/Documents`
  * `wget http://nginx.org/download/nginx-1.20.1.tar.gz`
  * `cd nginx-1.20.1`
  * `./configure --prefix=/usr/local/nginx --with-http_stub_status_module --with-http_ssl_module --with-stream`
  * `make`
  * `sudo make install`
  * `sudo gedit /usr/local/nginx/conf/nginx.conf`
  ```
  stream {
      upstream MyServer {
          server 127.0.0.1:6000 weight=1 max_fails=3 fail_timeout=30s;
          server 127.0.0.1:6002 weight=1 max_fails=3 fail_timeout=30s;
      }
      server {
          proxy_connect_timeout 1s;
          #proxy_timeout 3s;
          listen 8000;
          proxy_pass MyServer;
          tcp_nodelay on;
      }
  }
  ```
  * `sudo ln -s /usr/local/nginx/sbin/nginx  /usr/local/bin`
  * `sudo nginx -t -c /usr/local/nginx/conf/nginx.conf`
  * `sudo nginx`
  * `sudo nginx -s reload`
  * `sudo nginx -s stop`
  * `sudo netstat -tanp`
  * `kill pid`

