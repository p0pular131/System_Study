import requests
from bs4 import BeautifulSoup

songs = []

years = [20240605,20240505,20240630,20240531,20240430,20240331,20240229,20240131,20231231,20231130,20231031,20230930,20230831,20230731,20230630,20230531,20230430,20230331,20230228,20230131,20221231,20221130,20221031,20220930,20220831,20220731,20220630,20220531,20220430,20220331,20220228,20220131,20211231,20211130,20211031,20210930,20210831,20210731,20210630,20210531,20210430,20210331,20210228,20210131,20201231,20201130,20201031,20200930,20200831,20200731,20200630,20200531,20200430,20200331,20200229,20200131,20191231,20191130,20191031,20190930,20190831,20190731,20190630,20190531,20190430,20190331,20190228,20190131,20181231,20181130,20181031,20180930,20180831,20180731,20180630,20180531,20180430,20180331,20180228,20180131,20171231,20171130,20171031,20170930,20170831,20170731,20170630,20170531,20170430,20170331,20170228,20170131,20161231,20161130,20161031,20160930,20160831,20160731,20160630,20160531,20160430,20160331,20160229,20160131,20151231,20151130,20151031,20150930,20150831,20150731,20150630,20150531,20150430,20150331,20150228,20150131,20141231,20141130,20141031,20140930,20140831,20140731,20140630,20140531,20140430,20140331,20140228,20140131,20131231,20131130,20131031,20130930,20130831,20130731,20130630,20130531,20130430,20130331,20130228,20130131,20121231,20121130,20121031,20120930,20120831,20120731,20120630,20120531,20120430,20120331,20120229,20120131,20111231,20111130,20111031,20110930,20110831,20110731,20110630,20110531,20110430,20110331,20110228,20110131,20101231,20101130,20101031,20100930,20100831,20100731,20100630,20100531,20100430,20100331,20100228,20100131,20091231,20091130,20091031,20090930,20090831,20090731,20090630,20090531,20090430,20090331,20090228,20090131,20081231,20081130,20081031,20080930,20080831,20080731,20080630,20080531,20080430,20080331,20080229,20080131,20071231,20071130,20071031,20070930,20070831,20070731,20070630,20070531,20070430,20070331,20070228,20070131,20061231,20061130,20061031,20060930,20060831,20060731,20060630,20060531,20060430,20060331,20060228,20060131,20051231,20051130,20051031,20050930,20050831,20050731,20050630,20050531,20050430,20050331,20050228,20050131,20041231,20041130,20041031,20040930,20040831,20040731,20040630,20040531,20040430,20040331,20040229,20040131,20031231,20031130,20031031,20030930,20030831,20030731,20030630,20030531,20030430,20030331,20030228,20030131,20021231,20021130,20021031,20020930,20020831,20020731,20020630,20020531,20020430,20020331,20020228,20020131,20011231,20011130,20011031,20010930,20010831,20010731,20010630,20010531,20010430,20010331,20010228,20010131,20001231,20001130,20001031,20000930,20000831,20000731,20000630,20000531,20000430,20000331,20000229,20000131,19991231,19991130,19991031,19990930,19990831,19990731,19990630,19990531,19990430,19990331,19990228,19990131,19981231,19981130,19981031,19980930,19980831,19980731,19980630,19980531]

with open('songs.txt', 'w', encoding='utf-8') as f:
    for year in years :
        print(year)
        # URL을 읽어서 HTML을 받아온다.
        headers = {'User-Agent' : 'Mozilla/5.0 (Windows NT 10.0; Win64; x64)AppleWebKit/537.36 (KHTML, like Gecko) Chrome/73.0.3683.86 Safari/537.36'}
        data_kayo = requests.get('https://www.genie.co.kr/chart/genre?ditc=D&ymd='+str()+'&genrecode=M0100',headers=headers)
        data_pop = requests.get('https://www.genie.co.kr/chart/genre?ditc=D&ymd='+str()+'&genrecode=M0200',headers=headers)
        data_jpop = requests.get('https://www.genie.co.kr/chart/genre?ditc=D&ymd='+str()+'&genrecode=M0400',headers=headers)
        data_edm = requests.get('https://www.genie.co.kr/chart/genre?ditc=D&ymd='+str()+'&genrecode=E0000',headers=headers)

        # HTML을 BeautifulSoup이라는 라이브러리를 활용해 검색하기 용이한 상태로 만듦
        soup_kayo = BeautifulSoup(data_kayo.text, 'html.parser')
        soup_pop = BeautifulSoup(data_pop.text, 'html.parser')
        soup_jpop = BeautifulSoup(data_jpop.text, 'html.parser')
        soup_edm = BeautifulSoup(data_edm.text, 'html.parser')

        # select를 이용해서, tr들을 불러오기
        # BeautifulSoup의 select함수를 사용해 가져오고 싶은 부분을 가져올 수 있습니다.
        trs_kayo = soup_kayo.select('#body-content > div.newest-list > div > table > tbody > tr')
        trs_pop = soup_pop.select('#body-content > div.newest-list > div > table > tbody > tr')
        trs_jpop = soup_jpop.select('#body-content > div.newest-list > div > table > tbody > tr')
        trs_edm = soup_edm.select('#body-content > div.newest-list > div > table > tbody > tr')

        # 반복문으로 tr출력하기
        for tr in trs_kayo:
            a_tag = tr.select_one('td.info > a.title.ellipsis')
            # print(a_tag)
            if a_tag is not None:
                # 깔끔하게 출력하기 위해 strip()을 통해 양쪽 공백을 없애줍니다.
                title = a_tag.text.strip()
                genre = 'kayo'
                artist = tr.select_one('td.info > a.artist.ellipsis').text
                str_1 = genre + ", " + title + ", " + artist + ", " + str(year)[:4] + "\n"
                if (str_1[5:8] == "19금") :
                    continue
                songs.append(str_1)

        for tr in trs_pop:
            a_tag = tr.select_one('td.info > a.title.ellipsis')
            # print(a_tag)
            if a_tag is not None:
                # 깔끔하게 출력하기 위해 strip()을 통해 양쪽 공백을 없애줍니다.
                title = a_tag.text.strip()
                genre = 'pop'
                artist = tr.select_one('td.info > a.artist.ellipsis').text
                str_1 = genre + ", " + title + ", " + artist + ", " + str(year)[:4] + "\n"
                if (str_1[5:8] == "19금") :
                    continue
                songs.append(str_1)

        for tr in trs_jpop:
            a_tag = tr.select_one('td.info > a.title.ellipsis')
            # print(a_tag)
            if a_tag is not None:
                # 깔끔하게 출력하기 위해 strip()을 통해 양쪽 공백을 없애줍니다.
                title = a_tag.text.strip()
                genre = 'jpop'
                artist = tr.select_one('td.info > a.artist.ellipsis').text
                str_1 = genre + ", " + title + ", " + artist + ", " + str(year)[:4] + "\n"
                if (str_1[5:8] == "19금") :
                    continue
                songs.append(str_1)

        for tr in trs_edm:
            a_tag = tr.select_one('td.info > a.title.ellipsis')
            # print(a_tag)
            if a_tag is not None:
                # 깔끔하게 출력하기 위해 strip()을 통해 양쪽 공백을 없애줍니다.
                title = a_tag.text.strip()
                genre = 'edm'
                artist = tr.select_one('td.info > a.artist.ellipsis').text
                str_1 = genre + ", " + title + ", " + artist + ", " + str(year)[:4] + "\n"
                if (str_1[5:8] == "19금") :
                    continue
                songs.append(str_1)

    for song in songs :
        f.write(song)