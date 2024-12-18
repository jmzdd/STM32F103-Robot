import flet as ft
import paho.mqtt.client as mqtt
import threading

# MQTT配置
MQTT_BROKER = "47.243.62.42"
MQTT_PORT = 1883
MQTT_TOPIC = "ROBOT/CMD"
client_id = "111"
# Flet应用
def main(page: ft.Page):
    page.title = "MQTT 示例"

    # 消息显示区域
    message_box = ft.ListView()
    status_text = ft.Text("连接中...", size=16)

    def on_connect(client, userdata, flags, reason_code, properties):
        if reason_code == 0:
            status_text.value = "连接成功！"
            client.subscribe(MQTT_TOPIC)
        else:
            status_text.value = f"连接失败，代码: {reason_code}"
        page.update()

    def on_message(client, userdata, message):
        message_text = message.payload.decode()  # 解码消息
        message_box.add(ft.Text(message_text))
        page.update()

    # MQTT客户端
    def mqtt_client():
        client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2, client_id)  # 使用新的回调版本
        client.on_connect = on_connect  # 设置连接回调
        client.on_message = on_message    # 设置消息回调
        try:
            client.connect(MQTT_BROKER, MQTT_PORT, 60)
            client.loop_forever()
        except Exception as e:
            status_text.value = f"连接异常: {e}"
            page.update()

    # 启动MQTT线程
    threading.Thread(target=mqtt_client, daemon=True).start()

    # 添加界面元素
    page.add(ft.Column([status_text, message_box]))

# 运行Flet应用
ft.app(target=main)
