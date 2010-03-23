
function hostnameFromUrl(url) {
    return url.match(/:\/\/(.[^/]+)/)[1];
}
