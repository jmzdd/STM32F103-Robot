const express = require('express');
const http = require('http');
const net = require('net');

const app = express();
const server = http.createServer(app);

let latestImage = null;

const tcpServer = net.createServer((socket) => {
    console.log('ESP-CAM 连接成功！');
    let imageBuffer = Buffer.alloc(0);

    socket.on('data', (data) => {
        imageBuffer = Buffer.concat([imageBuffer, data]);
        let startIndex = imageBuffer.indexOf(Buffer.from([0xFF, 0xD8]));
        let endIndex = imageBuffer.indexOf(Buffer.from([0xFF, 0xD9]));
    
        while (startIndex !== -1 && endIndex !== -1) {
            const image = imageBuffer.slice(startIndex, endIndex + 2);
            if (image.length > 0) {
                latestImage = image;
                console.log(`提取的图像长度: ${latestImage.length}`);
            }
            
            imageBuffer = imageBuffer.slice(endIndex + 2);
            startIndex = imageBuffer.indexOf(Buffer.from([0xFF, 0xD8]));
            endIndex = imageBuffer.indexOf(Buffer.from([0xFF, 0xD9]));
        }
    });
    
    socket.on('end', () => {
        console.log('ESP-CAM 断开连接');
    });
});

tcpServer.listen(8001, () => {
    console.log('TCP 服务器正在监听 8001 端口');
});

// HTTP 路由
app.get('/video_feed', (req, res) => {
    res.setHeader('Content-Type', 'multipart/x-mixed-replace; boundary=frame');

    const interval = setInterval(() => {
        if (latestImage) {
            res.write(`--frame\r\n`);
            res.write(`Content-Type: image/jpeg\r\n`);
            res.write(`Content-Length: ${latestImage.length}\r\n\r\n`);
            res.write(latestImage);
            res.write(`\r\n`);
        } else {
            console.log('没有可发送的图像');
        }
    }, 100); // 修改为每100毫秒发送一帧图像

    req.on('close', () => {
        clearInterval(interval);
        res.end();
    });
});


// 主页面路由
app.get('/', (req, res) => {
    res.sendFile(__dirname + '/index.html');
});

// 启动 HTTP 服务器
server.listen(3000, () => {
    console.log('HTTP 服务器正在监听 3000 端口');
});
