import requests
from bs4 import BeautifulSoup

songs = []

years = [20240605, 20230605, 20220605, 20210605, 20200605, 20190605,
         20180605, 20170605, 20160605, 20150605, 20140605, 20130605, 
         20120605, 20110605, 20100605, 20090605, 20080605, 20070605]

with open('songs.txt', 'w', encoding='utf-8') as f:
    for year in years :
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