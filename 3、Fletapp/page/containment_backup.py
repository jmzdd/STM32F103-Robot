import flet as ft
from mqtt import Connect_MQTT
from mqtt import Send_Data

button_states = {
    "UP": False,
    "LEFT": False,
    "RIGHT": False,
    "DOWN": False,
}

def create_containment_page(page, nav_bar_height):
    Connect_MQTT()
    buttons = {}
    page.snack_bar = ft.SnackBar(content=ft.Text(""))

    def toggle_icon_button(e):
        current_key = e.control.key
        message = ""

        # Check current button state
        if button_states[current_key]:
            button_states[current_key] = False
            e.control.selected = False
            message = f"{current_key}按钮被按下, 启动状态"
            if current_key == "UP":
                Send_Data("QB")
            if current_key == "LEFT":
                Send_Data("ZB")
            if current_key == "RIGHT":
                Send_Data("YB")
            if current_key == "DOWN":
                Send_Data("HB")
        else:
            # Reset all buttons to unselected
            for key in button_states.keys():
                button_states[key] = False
                buttons[key].selected = False

            # Set current button as selected and change to red
            button_states[current_key] = True
            e.control.selected = True
            message = f"{current_key}按钮被按下, 停止状态"
            if current_key == "UP":
                Send_Data("QS")
            if current_key == "LEFT":
                Send_Data("ZS")
            if current_key == "RIGHT":
                Send_Data("YS")
            if current_key == "DOWN":
                Send_Data("HS")

        page.snack_bar.content = ft.Text(message)  # Update SnackBar content
        page.snack_bar.open = True  # Open SnackBar
        page.update()  # Update page

    # Create buttons and add to dictionary
    for key, icon in [("UP", ft.icons.ARROW_CIRCLE_UP),
                      ("LEFT", ft.icons.ARROW_CIRCLE_LEFT_OUTLINED),
                      ("RIGHT", ft.icons.ARROW_CIRCLE_RIGHT_OUTLINED),
                      ("DOWN", ft.icons.ARROW_CIRCLE_DOWN)]:
        buttons[key] = ft.IconButton(
            icon=icon,
            selected_icon=icon,
            selected=False,
            icon_size=70,
            on_click=toggle_icon_button,
            style=ft.ButtonStyle(color={"selected": ft.colors.RED, "": ft.colors.GREEN}),
            key=key,
        )
     # 这里的"ft.WebView"当中的网址应替换为自己的视频回传地址，图像流以网页的形式展示在网页上，在APP上以Webview控件的形式引入
    return ft.Column(
        spacing=5,
        controls=[
            ft.Row(
                alignment=ft.MainAxisAlignment.CENTER,
                controls=[
                    ft.WebView(
                    "https://camweb.jmzdd-jjly.website/",
                        width=245,
                        height=185
                    ),
                    # ft.Image(
                    #     src="https://picsum.photos/200/200?1",
                    #     width=245,
                    #     height=185,
                    # )
                ]
            ),
            ft.Row(
                alignment=ft.MainAxisAlignment.CENTER,
                controls=[buttons["UP"]]
            ),
            ft.Row(
                spacing=80,
                alignment=ft.MainAxisAlignment.CENTER,
                controls=[buttons["LEFT"], buttons["RIGHT"]]
            ),
            ft.Row(
                alignment=ft.MainAxisAlignment.CENTER,
                controls=[buttons["DOWN"]]
            ),
            # page.snack_bar  # Add SnackBar to the page controls

            # 添加一个空元素来增大空隙
            ft.Container(height=20),  # 设置高度为20像素的间隔

            # 添加一个终端界面用来输出信息
            # ft.Row(
            #     alignment=ft.MainAxisAlignment.CENTER,
            #     controls=[
            #         # 添加一个容器
            #         ft.Container(
            #             width=300,
            #             height=150,
            #             bgcolor="#212121",
            #             border_radius=10,
            #             content=ft.Column(
            #                 spacing=5,
            #                 controls=[
            #                     ft.Container(height=2),  # 这里可以调整高度以改变位置
            #                     ft.Row(
            #                         spacing=5,
            #                         alignment=ft.MainAxisAlignment.START,
            #                         controls=[
            #                             ft.Container(width=2),  # 这里可以调整宽度以改变位置
            #                             ft.Icon(name=ft.icons.CIRCLE, color=ft.colors.RED, size=20),
            #                             ft.Icon(name=ft.icons.CIRCLE, color=ft.colors.YELLOW, size=20),
            #                             ft.Icon(name=ft.icons.CIRCLE, color=ft.colors.GREEN, size=20),
            #                         ]
            #                     ),
            #                     ft.Divider(height=9, thickness=3),
            #                     ft.Container(height=2),  # 这里可以调整高度以改变位置
            #                     ft.ListView(expand=1, spacing=5, padding=20, auto_scroll=True),
            #                 ]
            #             ),
            #         )
            #     ]
            # ),
        ]
    )
