import adlink
print(dir(adlink))
print(adlink.sin2(1))
ad=adlink.Adlink()
ad.link()
print(ad.getdi())
