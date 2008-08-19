function TicketList()
result=".gmt get";   
outSAY(result);
end

function TicketChan()
JoinChannelByName("gm_sync_channel", "lhjf448gfdw279hgfw6");
end

function ShowTicket()
result=".gmt getid "..TicketNumber:GetText();   
outSAY(result);
end

function DeleteTicket()
result=".gmt delid "..TicketNumber:GetText();    
outSAY(result);
end

