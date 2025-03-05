def on_halted(pc): 
    bp = memPgm.Read16(pc) 
    if bp != 48811: #0xbeab 
        return False 
    semihost() 
    deb.SetNamedReg("PC", pc+2) #move pc over swbp 
    return True 

def semihost(): 
    r = deb.GetNamedReg("R0") 
    if r == 12: 
        r = deb.SetNamedReg("R0",1) #file length 1 for :tt 
        return  
    
    if r == 5:         
        block = deb.GetNamedReg("R1") 
        buf = memPgm.Read32(block+4) 
        len = memPgm.Read16(block+8) 
        message = bytearray(len) 
        memPgm.Read(buf ,message,0,len) 
        msg.printToTab(message.decode(), "Printf output") 
 
    r = deb.SetNamedReg("R0",0)  
    return
