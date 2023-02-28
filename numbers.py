import framebuf

# height 14
# width 8

def draw(framebuffer, x, y, number):
    if number==1:
        framebuffer.vline(x+5, y, 14, 0xffff)
    elif number==2:
        framebuffer.hline(x+1, y, 7, 0xffff)
        framebuffer.vline(x+8, y, 7, 0xffff)
        framebuffer.hline(x+1, y+7, 7, 0xffff)
        framebuffer.vline(x, y+8, 7, 0xffff)
        framebuffer.hline(x+1,  y+14, 7, 0xffff)
    elif number==3:
        framebuffer.hline(x+1, y, 7, 0xffff)
        framebuffer.vline(x+8, y, 14, 0xffff)
        framebuffer.hline(x+1, y+7, 7, 0xffff)
        framebuffer.hline(x+1,  y+14, 7, 0xffff)
    elif number==4:
        framebuffer.vline(x, y, 7, 0xffff)
        framebuffer.vline(x+8, y, 14, 0xffff)
        framebuffer.hline(x+1, y+7, 7, 0xffff)
    elif number==5:
        framebuffer.vline(x, y, 7, 0xffff)
        framebuffer.hline(x+1, y, 7, 0xffff)
        framebuffer.vline(x+8, y+7, 7, 0xffff)
        framebuffer.hline(x+1, y+7, 7, 0xffff)
        framebuffer.hline(x+1, y+14, 7, 0xffff)
    elif number==6:
        framebuffer.vline(x, y, 14, 0xffff)
        framebuffer.hline(x+1, y, 7, 0xffff)
        framebuffer.vline(x+8, y+7, 7, 0xffff)
        framebuffer.hline(x+1, y+7, 7, 0xffff)
        framebuffer.hline(x+1,  y+14, 7, 0xffff)
    elif number==7:
        framebuffer.hline(x+1, y, 7, 0xffff)
        framebuffer.vline(x+8, y, 14, 0xffff)
    elif number==8:
        framebuffer.vline(x, y, 14, 0xffff)
        framebuffer.hline(x+1, y, 7, 0xffff)
        framebuffer.vline(x+8, y, 14, 0xffff)
        framebuffer.hline(x+1, y+7, 7, 0xffff)
        framebuffer.hline(x+1, y+14, 7, 0xffff)
    elif number==9:
        framebuffer.vline(x, y, 7, 0xffff)
        framebuffer.hline(x+1, y, 7, 0xffff)
        framebuffer.vline(x+8, y, 14, 0xffff)
        framebuffer.hline(x+1, y+7, 7, 0xffff)
        framebuffer.hline(x+1,  y+14, 7, 0xffff)
    elif number==0:
        framebuffer.vline(x, y, 14, 0xffff)
        framebuffer.hline(x+1, y, 7, 0xffff)
        framebuffer.vline(x+8, y, 14, 0xffff)
        framebuffer.hline(x+1,  y+14, 7, 0xffff)
    else:  # unknown number
        framebuffer.hline(x+1, y, 7, 0xffff)
        framebuffer.hline(x+1, y+7, 7, 0xffff)
        framebuffer.hline(x+1,  y+14, 7, 0xffff) 


