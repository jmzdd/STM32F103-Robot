import flet as ft
from page import fonts
from page.home import create_home_page
from page.containment import create_containment_page
# from page.digital import create_digital_page
# from page.setting import create_setting_page

def main(page: ft.Page):
    # 设置页面标题
    page.title = "Robot CMD"
    page.scroll = "adaptive"

    # 控制运行后的页面大小
    # page.window_width = 500  # 设置窗口宽度
    # page.window_height = 1000  # 设置窗口高度

    # 设置全局AppBar
    page.appbar = ft.AppBar(
        leading_width=40,
        title=ft.Text("Robot CMD", weight=ft.FontWeight.BOLD),
        center_title=True,
        bgcolor=ft.colors.SURFACE_VARIANT,
        actions=[
            # 点击事件通过 lambda 函数触发页面的 page.open(dlg_modal) 方法，打开对话框
            ft.IconButton(ft.icons.INFO, on_click=lambda e: page.open(dlg_modal)),

            # 使用空容器隔开
            ft.Container(width=10)
        ],
    )

    def handle_close(e):
        page.close(dlg_modal)

    # “关于”页面弹窗
    dlg_modal = ft.AlertDialog(
        modal=True,
        title=ft.Text("关于本应用", weight=ft.FontWeight.BOLD, font_family="MiSans"),
        content=ft.Column(
            [
                ft.Text(spans=[
                    ft.TextSpan("Github主页："),
                    ft.TextSpan(
                        "jmzdd",
                        ft.TextStyle(decoration=ft.TextDecoration.UNDERLINE, weight=ft.FontWeight.NORMAL, size=15, color="#5abae7"),
                        url="https://github.com/jmzdd",
                        on_enter=fonts.highlight_link,
                        on_exit=fonts.unhighlight_link
                    ),
                ]),
                ft.Text("版本号：v1.0.0", weight=ft.FontWeight.NORMAL, font_family="MiSans")
            ],
            tight=True  # 控件紧密排列，不增加额外间距
        ),
        actions=[
            ft.TextButton("好的", on_click=handle_close),
        ],
        actions_alignment=ft.MainAxisAlignment.END,
    )


    # 先全局隐藏AppBar
    page.appbar.visible = False  # 隐藏 AppBar

    # 创建欢迎页面
    welcome_page = ft.Stack([
        ft.Container(
            content=ft.Column([
                ft.Text("欢迎使用本应用！", size=25, weight=ft.FontWeight.BOLD, font_family="MiSans"),
                ft.Text(
                    disabled=False,
                    size=15,
                    weight=ft.FontWeight.NORMAL,
                    font_family="MiSans",
                    spans=[
                        ft.TextSpan("这是Robot的移动端控制App，使用"),
                        ft.TextSpan(
                            "Flet",
                            ft.TextStyle(decoration=ft.TextDecoration.UNDERLINE, weight=ft.FontWeight.BOLD, size=15, color="#5abae7"),
                            url="https://flet.dev",
                            on_enter=fonts.highlight_link,
                            on_exit=fonts.unhighlight_link
                        ),
                        ft.TextSpan("构建"),
                    ],
                ),
                ft.Text("本App仅用于本机器人的调试控制，不作其他用途", size=15),
                ft.Image(
                    src=f"./assets/Flet_logo.png",
                    width=100,
                    height=100
                ),
                ft.Container(height=25),  # 使用Container来创建间距
                ft.ElevatedButton("进入应用", style=ft.ButtonStyle(text_style=ft.TextStyle(color=ft.colors.BLACK, font_family="MiSans")), icon="START", on_click=lambda e: switch_to_main(e))
            ],
            alignment=ft.MainAxisAlignment.CENTER,
            horizontal_alignment=ft.CrossAxisAlignment.CENTER),
            alignment=ft.alignment.center,
            width=page.width,
            height=page.height
        )
    ])

    # 创建一个容器来显示不同的内容
    content_container = ft.Container()

    # 创建导航栏，并设置初始选中项
    navigation_bar = ft.NavigationBar(
        destinations=[
            ft.NavigationBarDestination(icon=ft.icons.DOCUMENT_SCANNER, label="说明"),
            ft.NavigationBarDestination(icon=ft.icons.KEYBOARD_COMMAND_KEY, label="控制"),
            # ft.NavigationBarDestination(icon=ft.icons.DATA_THRESHOLDING, label="数据"),
            # ft.NavigationBarDestination(icon=ft.icons.SETTINGS, label="设置"),
        ],
        selected_index=0  # 设置初始选中项为 0
    )

    # 获取导航栏的高度
    nav_bar_height = navigation_bar.height or 0  # 如果高度为 None，则使用 0

    # 定义一个字典来存储每个页面的内容
    page_contents = {
        "说明": create_home_page(page, nav_bar_height),
        "控制": create_containment_page(page, nav_bar_height),
        # "数据": create_digital_page(page, nav_bar_height),
        # "设置": create_setting_page(page, nav_bar_height)
    }

    def update_content(event):
        # 获取当前选中的导航项索引
        selected_index = navigation_bar.selected_index
        destination = navigation_bar.destinations[selected_index]

        # 更新容器中的内容
        if destination.label in page_contents:
            content_container.content = page_contents[destination.label]

        # 刷新页面以显示新内容
        page.update()

    # 将事件处理器绑定到导航栏的改变事件
    navigation_bar.on_change = update_content

    # 初始化默认内容
    update_content(None)  # 调用一次 update_content 函数以初始化默认内容

    # 添加欢迎页面
    page.add(welcome_page)

    def switch_to_main(e):
        # 移除欢迎页面
        page.controls.remove(welcome_page)

        # 将导航栏和容器添加到页面
        page.add(navigation_bar, content_container)

        # 设置 AppBar 为可见
        page.appbar.visible = True
        page.update()

ft.app(target=main)
