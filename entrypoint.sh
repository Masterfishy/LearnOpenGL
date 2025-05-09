#!/bin/sh

# Start nginx
nginx

# Watches for changes to the build artifacts
echo "Watching for changes..."
while true; do
    inotifywait -r -e modify,create,delete,move /usr/share/nginx/html
    clear
    echo "Change detected, reloading nginx..."
    nginx -s reload
done
