(function() {
    var cbs = [];
    var conn = new QWebChannel(qt.webChannelTransport, function() {
        for (var i = 0; i < cbs.length; i++)
            cbs[i](conn);
        cbs = null;
    });
    function withConn(cb) {
        if (cbs !== null)
            cbs.push(cb);
        else
            cb(conn);
    }

    window.mm = {};
    window.mm.showView = function() {
        withConn(function(conn) {
            conn.objects.loginWindow.showWebBrowser();
        });
    };
    window.mm.setAccountIdentifier = function(identifer) {
        withConn(function(conn) {
            conn.objects.loginWindow.setAccountIdentifier(identifer);
        });
    };
    window.mm.log = function(what) {
        console.log(what);
    };
})();
