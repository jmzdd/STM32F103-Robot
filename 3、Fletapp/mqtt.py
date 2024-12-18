import paho.mqtt.client as mqtt

# MQTT 服务器的地址和端口
# MQTT服务器的IP地址
MQTT_BROKER = "YOU SERVER IP"  # 可以使用公共的 MQTT 代理
# MQTT服务器的端口号
MQTT_PORT = 0000
# 订阅的Topic
MQTT_TOPIC = "XXX"
# 客户端ID
CLIENT_ID = "XXX"

# 全局变量，用于存储MQTT客户端实例
client = None

def Connect_MQTT():
    global client
    if client is None or not client.is_connected():
        client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2, CLIENT_ID) # 使用新的回调版本
        client.connect(MQTT_BROKER, MQTT_PORT, keepalive=60)
        client.loop_start()
    return client

def Send_Data(message):
    client = Connect_MQTT()
    client.publish(MQTT_TOPIC, message)
