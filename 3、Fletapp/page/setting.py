import flet as ft

def create_setting_page(page, nav_bar_height):
    return ft.Stack(
            [
                ft.Container(
                    content=ft.Text("这是 设置 页面的文字", text_align=ft.TextAlign.CENTER),
                    alignment=ft.alignment.top_center,
                    width=page.width,
                    height=page.height - nav_bar_height
                )
            ]
        )
