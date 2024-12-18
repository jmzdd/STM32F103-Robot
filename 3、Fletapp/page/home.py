import flet as ft

md1 = """
# App使用向导
- 本页面将引导您如何使用本App
- 机器人分有两种控制模式：本地控制模式、远程控制模式

### 一、本地控制模式（配网模式）
- 连接由机器人发出的AP热点，AP热点名称为：Robot Config AP
- 连接至AP热点后，使用浏览器打开[网页](http://192.168.4.1/)
- 之后即可访问机器人的本地设置页面

### 二、远程控制模式
⚠注意：需要先通过配网模式，让机器人连接网络
- 机器人在成功联网后，即可点击本App的“控制”选项卡，对机器人进行控制
"""

def create_home_page(page, nav_bar_height):
    return ft.Markdown(
            md1,
            selectable=True,
            extension_set=ft.MarkdownExtensionSet.GITHUB_WEB,
            on_tap_link=lambda e: page.launch_url(e.data),
        )
