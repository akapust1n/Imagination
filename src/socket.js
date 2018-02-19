import {w3cwebsocket as W3CWebSocket} from "websocket";

let Socket = (function () {
    let instance;

    function createInstance() {
        let socket = new W3CWebSocket("wss://kapust1n.ru:5000/ws");
        socket.onopen = () => console.log("Соединение установлено.");
        socket.onclose = event => {
            if (event.wasClean) {
                console.log('Соединение закрыто чисто');
            } else {
                console.log('Обрыв соединения');
            }
            console.log('Код: ' + event.code + ' причина: ' + event.reason);
        };
        socket.onerror = error => console.log("Ошибка " + error.message);
        socket.handlers = {};
        socket.addHandler = function (type, handler) {
            socket.handlers[type] = handler;
        };
        socket.onmessage = event => {
            console.log("Получены данные " + event.data);
            let data = JSON.parse(event.data);
            let type = data.type;
            if (!type) {
                return;
            }
            let handler = socket.handlers[type];
            if (handler) {
                handler(data.content);
            }
        };
        return socket;
    }

    return {
        getInstance: function () {
            if (!instance) {
                instance = createInstance();
            }
            return instance;
        }
    };
})();

export default Socket;