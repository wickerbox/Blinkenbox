mycounter=0
srv=net.createServer(net.TCP) 
srv:listen(80,function(conn) 
conn:on("receive",function(conn,payload) 
   if string.find(payload,"?myarg=") then
   mycounter=mycounter+1
    m="<br/>Value= " .. string.sub(payload,string.find(payload,"?myarg=")+7,string.find(payload,"HTTP")-2)
  else 
    m=""
 end
 conn:send("<h1> Hello, this is Androiders's web page.</h1>How are you today.<br/> Count=" .. mycounter .. m .. "Heap="
.. node.heap()) 
 end) 
conn:on("sent",function(conn) conn:close() end) 
end)
