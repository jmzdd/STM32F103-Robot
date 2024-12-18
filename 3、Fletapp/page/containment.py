import flet as ft
from mqtt import Connect_MQTT, Send_Data

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
                buttons[key].update()  # 更新每个按钮的状态

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

        page.snack_bar.content.value = message  # Update SnackBar content
        page.snack_bar.open = True  # Open SnackBar
        e.control.update()  # 更新当前按钮状态
        page.snack_bar.update()  # 更新SnackBar状态，不刷新整个页面

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
                    )
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
            ft.Container(height=20),  # 设置高度为20像素的间隔
        ]
    )
