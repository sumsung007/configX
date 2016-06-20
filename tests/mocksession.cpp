#include "mocksession.hpp"

#include <QtCore/QDebug>

MockSession::MockSession():
    NetworkSession("", 0)
{

}

bool MockSession::readFileContents(const QString path, QString *content)
{
    qDebug() << "Reading file" << *content;

    if (path.compare("/etc/nginx/nginx.conf", Qt::CaseInsensitive) == 0) {
        *content = "user www-data; worker_processes auto; pid /var/run/nginx.pid; events { worker_connections 768; } http { server_tokens off; server_names_hash_bucket_size 64; rewrite_log on; sendfile on; tcp_nopush on; tcp_nodelay on; keepalive_timeout 60; types_hash_max_size 2048; autoindex off; client_max_body_size 50M; include /etc/nginx/mime.types; default_type application/octet-stream; access_log /var/log/nginx/access.log; error_log /var/log/nginx/error.log notice; gzip on; gzip_disable \"msie6\"; gzip_vary on; gzip_proxied any; gzip_comp_level 6; gzip_buffers 16 8k; gzip_http_version 1.1; gzip_types text/plain text/css application/json application/x-javascript text/xml application/xml application/xml+rss text/javascript; include /etc/nginx/conf.d/*.conf; include /etc/nginx/vhosts/*.conf; }";
    } else {
        return false;
    }

    return true;
}
