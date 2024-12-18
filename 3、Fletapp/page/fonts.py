import flet as ft
def highlight_link(e):
    e.control.style.color = None
    e.control.update()


def unhighlight_link(e):
    e.control.style.color = ft.colors.BLUE
    e.control.update()