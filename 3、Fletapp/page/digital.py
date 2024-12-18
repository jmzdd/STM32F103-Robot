import flet as ft

def create_digital_page(page, nav_bar_height):
    return ft.Stack(
            [
                ft.Container(
                    content=ft.Text("这是 探索 页面的文字", text_align=ft.TextAlign.CENTER),
                    alignment=ft.alignment.top_center,
                    width=page.width,
                    height=page.height - nav_bar_height
                )
            ]
        )
