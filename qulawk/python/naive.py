import arrow

sessions=['Hypnos', 'Hemera', 'Aether', 'Hesperides', 'Nyx']
periods = ['Geef', 'Lep', 'Shad', 'Kwok', 'Zin']

psttime = arrow.utcnow().to('-08:00')
secs = psttime.second  + (0.000001 * psttime.microsecond) + (60 * psttime.minute) + (60 * 60 * psttime.hour)

session = int(secs // 17280)
secs = secs % 17280
period = int(secs // 3456)
secs = secs % 3456
bit = int(secs // 138)
secs = secs % 138
moment = int(secs // 5.52)

print "%s:%s:%s.%s" % (session, period, bit, moment)
print "%s %s'%s.%s" % (sessions[session], periods[period], bit, moment)
