import adlink
print(dir(adlink))
#print(adlink.plusOne("echo 'hello'",6))
r=adlink.link()
print("=======")
print(r)
print(adlink.getDO())
print(adlink.getDI())
print(adlink.getchanelVolt(0))
#print(adlink.putchanelVolt(0,-10))
#print(adlink.plusOne(1))