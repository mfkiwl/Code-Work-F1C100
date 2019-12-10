# encoding:utf-8
import requests

# client_id 为官网获取的AK， client_secret 为官网获取的SK
host = 'https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=O2GcufephToGTnlksxeKXPeX&client_secret=GD8eLCUhTNDlrp6lPyxGvUmk3cYE6jw6'
response = requests.get(host)
if response:
    print(response.json())

# {'refresh_token': '25.55ea9a0934408c584b02600130d5830c.315360000.1889428179.282335-17797198', 'expires_in': 2592000, 'session_key': '9mzdDcTlIbnoVOD1fysub1GEVCOJ/ACmlPHB9OABwmrps9dPlkBXb/QAcq08nYOk9JL2+bBRixV5iG9VWzCHSlW0vudpFQ==', 'access_token': '24.62f74cccea9d581faff710460634b806.2592000.1576660179.282335-17797198', 'scope': 'ai_custom_jch_car_detect public brain_all_scope easydl_mgr easydl_retail_mgr ai_custom_retail_image_stitch wise_adapt lebo_resource_base lightservice_public hetu_basic lightcms_map_poi kaidian_kaidian ApsMisTest_Test权限 vis-classify_flower lpq_开放 cop_helloScope ApsMis_fangdi_permission smartapp_snsapi_base iop_autocar oauth_tp_app smartapp_smart_game_openapi oauth_sessionkey smartapp_swanid_verify smartapp_opensource_openapi smartapp_opensource_recapi fake_face_detect_开放Scope vis-ocr_虚拟人物助理 idl-video_虚拟人物助理', 'session_secret': '9d5e582108cf058021716e1bf3ae89c9'}
