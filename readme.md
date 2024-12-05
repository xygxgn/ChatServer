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

* nginx
  * `cd ~/Documents`
  * `git clone https://github.com/nginx/nginx.git`
  * `cd nginx`
  * `./auto/configure --with-stream`
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

