#!/usr/bin/env python
# -*- coding: utf-8 -*-
# import urllib2
import chardet
import requests
from urllib import parse
import json
import time



def urldecode(purl):
    return parse.unquote(purl)
def urlencode(purl):
    return parse.quote(purl)

def conventStrTOUtf8(oldstr):
    try:
        nstr = oldstr.encode("utf-8")
        return nstr
    except Exception as e:
        print('nstr do not encode utf-8')
    cnstrtype = chardet.detect(oldstr)['encoding']
    utf8str =  oldstr.decode(cnstrtype).encode('utf-8')
    return utf8str

def postDataToURL(purl,data):
    if purl[0:5] == 'https':
        response = requests.post(purl,data=data,verify=False)
        dat = response.text
        return dat
    else:
        response = requests.post(purl,data=data)
        dat = response.text
        return dat

def getUrl(purl):
    try:
        if purl[0:5] == 'https':
            res = requests.get(purl, verify=False)
            # print(res.text)
            return res.text
        else:
            res = requests.get(purl)
            # print(res.text)
            return res.text
    except Exception as e:
        print(e)
    return None

def getUrlWithChrome(purl,isVerify = False,isProxy=False):
    rurl = purl
    proxies = { "http": "http://127.0.0.1:1080", "https": "http://127.0.0.1:1080" }
    try:
        s = requests.Session()
        s.headers.update({'user-agent': 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/62.0.3202.94 Safari/537.36'})
        s.headers.update({'Host': 'www.okx.com'})
        
        if purl[0:5] == 'https':
            res = None
            if isProxy:
                res = s.get(rurl,verify=isVerify,proxies=proxies)
            else:
                res = s.get(rurl,verify=isVerify)
            # print(res.text)
            return res.text
        else:
            res = None
            if isProxy:
                res = requests.get(purl,proxies=proxies)
            else:
                res = requests.get(purl)
            # print(res.text)
            return res.text
    except Exception as e:
        print(e)
    return None

# {"code":"0","msg":"","data":[{"instId":"BTC-USD","idxPx":"29315.1","high24h":"30005.9","sodUtc0":"29511.8","open24h":"29698.6","low24h":"28007.3","sodUtc8":"29602","ts":"1653579015362"}]}
def getZhiShu():
    btxt = getUrlWithChrome(purl="https://www.okx.com/api/v5/market/index-tickers?instId=BTC-USD",isVerify = True)
    print(btxt)
    return btxt
def getCJHY():
    btxt = getUrlWithChrome(purl="https://www.okx.com/api/v5/market/tickers?instType=FUTURES&uly=BTC-USD",isVerify = True)
    print(btxt)
    return btxt

zhishuJson = '''
{
  "code": "0",
  "msg": "",
  "data": [
    {
      "instId": "BTC-USD",
      "idxPx": "29295",
      "high24h": "30005.9",
      "sodUtc0": "29511.8",
      "open24h": "29668.5",
      "low24h": "28007.3",
      "sodUtc8": "29602",
      "ts": "1653579227079"
    }
  ]
}
'''
def conventZhiSHUJsonData(jstr):
    jdic = json.loads(jstr)
    try:
        return float(jdic['data'][0]['idxPx'])
    except Exception as e:
        return 0;

cjhyJson = '''
{
  "code": "0",
  "msg": "",
  "data": [
    {
      "instType": "FUTURES",
      "instId": "BTC-USD-220930",
      "last": "29564.7",
      "lastSz": "5",
      "askPx": "29555",
      "askSz": "9",
      "bidPx": "29547.8",
      "bidSz": "1",
      "open24h": "29916.7",
      "high24h": "30282.4",
      "low24h": "28091.5",
      "volCcy24h": "3266.8615",
      "vol24h": "958323",
      "ts": "1653579225736",
      "sodUtc0": "29768.7",
      "sodUtc8": "29872.1"
    },
    {
      "instType": "FUTURES",
      "instId": "BTC-USD-220527",
      "last": "29294.2",
      "lastSz": "60",
      "askPx": "29298.8",
      "askSz": "51",
      "bidPx": "29290.8",
      "bidSz": "478",
      "open24h": "29644.1",
      "high24h": "30038.5",
      "low24h": "27971.6",
      "volCcy24h": "2042.1362",
      "vol24h": "598691",
      "ts": "1653579213957",
      "sodUtc0": "29495.9",
      "sodUtc8": "29593"
    },
    {
      "instType": "FUTURES",
      "instId": "BTC-USD-220624",
      "last": "29283.2",
      "lastSz": "4",
      "askPx": "29280",
      "askSz": "1",
      "bidPx": "29275",
      "bidSz": "1",
      "open24h": "29638.8",
      "high24h": "30000",
      "low24h": "27859.9",
      "volCcy24h": "8215.9444",
      "vol24h": "2393267",
      "ts": "1653579225817",
      "sodUtc0": "29470",
      "sodUtc8": "29570.1"
    },
    {
      "instType": "FUTURES",
      "instId": "BTC-USD-220603",
      "last": "29279.9",
      "lastSz": "13",
      "askPx": "29279.9",
      "askSz": "13",
      "bidPx": "29276.4",
      "bidSz": "5",
      "open24h": "29637.7",
      "high24h": "30003.6",
      "low24h": "27960",
      "volCcy24h": "1288.414",
      "vol24h": "376889",
      "ts": "1653579215419",
      "sodUtc0": "29505.6",
      "sodUtc8": "29582.4"
    }
  ]
}
'''

def conventCJHYJsonData(jstr):
    jdic = json.loads(jstr)
    maxdate = 0
    cjprice = 0
    try:
        data = jdic['data']
        for i,v in enumerate(data):
            dtmp = v['instId']
            datetmp = int(dtmp.split('-')[-1])
            if datetmp > maxdate:
                maxdate = datetmp
                cjprice = float(v['last'])
        return maxdate,cjprice
    except Exception as e:
        return 0,0

def saveToFile(ddic,fname = 'p.json'):
    spth = '/var/www/html/' + fname
    dstr = json.dumps(ddic)
    f = open(spth,'w')
    f.write(dstr)
    f.close()

def getDataFromOkx():
    # getZhiShu()
    # getCJHY()
    jstr = getZhiShu()
    # jstr = zhishuJson
    nprice = conventZhiSHUJsonData(jstr)
    print(nprice)
    time.sleep(0.3)
    jstr = getCJHY()
    # jstr = cjhyJson
    mdate,cjprice = conventCJHYJsonData(jstr)
    print(mdate,cjprice)
    percent = 0.0
    outdic = {}
    

    if nprice and mdate and cjprice:
        percent = (cjprice - nprice)/nprice
        print(percent)
        outdic['percent'] = float('%.4f'%(percent))
        outdic['price'] = nprice
        outdic['cjprice'] = cjprice
        outdic['cjdate'] = mdate
        outdic['erro'] = 0
        if percent > 0:
            outdic['zeng'] = 1 #是否是正数
        else:
            outdic['zeng'] = 0 #是否是负数
    else:
        print('erro')
        outdic['erro'] = 1
    saveToFile(outdic)

def main():
    while True:
        getDataFromOkx()
        time.sleep(300) #每5分钟请求一次

if __name__ == '__main__':
    main()


