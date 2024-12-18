from lib.microdot import Microdot
from lib.microdot import send_file
from lib.microdot import redirect
import network
import time
import camera
import usocket
from lib.umqtt.simple import MQTTClient
import _thread
client = None

# MQTT连接信息
mqtt_server = 'XXXXXXXX'
mqtt_port = 'XXXX'
client_id = 'XXX'
topic = 'XXX'

# MQTT Functions
def wait_for_messages(client):
    while True:
        client.wait_msg()
def message_callback(topic, msg):
    print(msg.decode('utf-8'))
def ConnectMQTTServer():
    global client
    client = MQTTClient(client_id, mqtt_server, port=mqtt_port)
    client.set_callback(message_callback)
    try:
        client.connect()
        client.subscribe(topic)
        _thread.start_new_thread(wait_for_messages, (client,))
        print("MQTT ok")
        return client
    except Exception as e:
        print("MQTT error:", e)
        return None

#相机初始化
for i in range(5):
    cam = camera.init(0, format=camera.JPEG, fb_location=camera.PSRAM)
    # 上下翻转
    # camera.flip(1)
    # 左右翻转
    # camera.mirror(1)
    camera.framesize(camera.FRAME_HQVGA)
    print("Camera ready? ", cam)
    if cam:
        print("Camera is ready.")
        break
    else:
        time.sleep(2)

app = Microdot()

# 处理前端访问请求
@app.route('/')
def main(request):
  return send_file('main.html')

@app.route('/command')
def command(request):
  return send_file('command.html')

@app.route('/wifi_info')
def wifi_info(request):
  return send_file('wifi_info.html')

# 接收前端访问请求
@app.get('/QB')
def QB(request):
  print("QB")
  
@app.get('/HB')
def HB(request):
  print("HB")

@app.get('/ZB')
def ZB(request):
  print("ZB")

@app.get('/YB')
def YB(request):
  print("YB")

@app.get('/QS')
def QS(request):
  print("QS")
  
@app.get('/HS')
def HS(request):
  print("HS")

@app.get('/ZS')
def ZS(request):
  print("ZS")

@app.get('/YS')
def YS(request):
  print("YS")

@app.get('/connect_wifi')
def success_page(request):
  WIFI_Connect()

# get wifi info
@app.post('/success_page')
def handle_form_submission(request):
    global ssid
    global password
    # 处理前端表单提交数据
    form_data = request.form
    ssid = form_data.get('ssid')
    password = form_data.get('password')
    print(ssid, password)
    # return redirect('/success_page')
    return send_file('success_page.html')

  
@app.route('/video_feed')
def video_feed(request):
    def stream():
        yield b'--frame\r\n'
        while True:
            frame = camera.capture()
            yield b'Content-Type: image/jpeg\r\n\r\n' + frame + \
                b'\r\n--frame\r\n'
    return stream(), 200, {'Content-Type':
                           'multipart/x-mixed-replace; boundary=frame'}

# IP地址为192.168.4.1
def AP_Connect():
    print("ap_mode")
    global AP_State
    AP_State = network.WLAN(network.AP_IF)
    AP_State.active(True)
    # AP_State.config(essid = 'Robots Config AP', authmode=network.AUTH_WPA_WPA2_PSK, password = '12345678')
    AP_State.config(essid = 'Robots Config AP')
    
    while AP_State.isconnected() != True:
        time.sleep(1)
        print("device_no_connect")
    print("have_device_connected")
    time.sleep(1)

def WIFI_Connect():
    AP_State.active(False)
    # 连接wifi
    sta_if = network.WLAN(network.STA_IF)
    if not sta_if.isconnected():
        print('connecting to network...')
        sta_if.active(True)
        sta_if.connect(ssid, password)
        while not sta_if.isconnected():
            pass
    print("connect!")
    ConnectMQTTServer()
    SendPhoto()

def SendPhoto():
    # server_ip = '192.168.31.235'  # 替换为你的服务器IP
    server_ip = 'XXXX' # 云服务器IP
    server_port = 8001

    print('服务器IP:',server_ip)
    print('端口号:',server_port)
    
    while True:
        try:
            s = usocket.socket(usocket.AF_INET, usocket.SOCK_STREAM)
            s.connect((server_ip, server_port))
            print("成功连接服务器！")

            while True:
                # 捕获 JPEG 图像
                frame = camera.capture()  
                if frame:
                    s.send(b'--frame\r\n')
                    s.send(b'Content-Type: image/jpeg\r\n')
                    s.send(b'Content-Length: ' + str(len(frame)).encode() + b'\r\n\r\n')
                    s.send(frame)
                    s.send(b'\r\n')
                    # print(f"Sent image of length: {len(frame)}")
                else:
                    print("重新获取到图像数据")
                # time.sleep(0.01)  # 控制发送频率
                client.check_msg()
        except OSError as e:
            print("没有连接成功:", e)
            time.sleep(5)  # 等待 3 秒后重连
        finally:
            if s:
                s.close()
            print("连接已关闭，准备重新连接...")
            time.sleep(1)  # 给一点时间重新连接

    
          
if __name__ == '__main__':
  AP_Connect()
  app.run(port=80)
