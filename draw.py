# coding=gbk
import matplotlib.pyplot as plt
import numpy as np
from pyecharts.charts import Bar
from pyecharts.render import make_snapshot
from pyecharts import options as opts
from pyecharts.globals import ThemeType
from pyecharts.commons.utils import JsCode

# import pyecharts.options as opts
from pyecharts.charts import Line
from pyecharts.faker import Faker


# 使用 snapshot-selenium 渲染图片
from snapshot_selenium import snapshot
from pyecharts.globals import CurrentConfig

import pandas as pd

CurrentConfig.ONLINE_HOST = "D:\\js\\"

fileName="python_data.txt"
modelName=["Rein","ReinBits","ReinBits1","ReinBits2"]
colorName=["#ff427f", "#9400D3", "#00BFFF","#808000"]
numModel=len(modelName)
numExp=9
data = [[0] * numModel for _ in range(numExp)]

df=pd.read_table(fileName,names=modelName)

p = [0,1,2,3,4,5,6,7,8]

#list(df[modelName[0]])



c = (
    Line(init_opts=opts.InitOpts(theme=ThemeType.ROMA))
    # theme=ThemeType.LIGHT INFOGRAPHIC ROMA WESTEROS
    .add_xaxis(p).add_yaxis(
        series_name=modelName[0],
        y_axis=list(df[modelName[0]]),
        label_opts=opts.LabelOpts(font_size=8,
                                  is_show=True,
                                  position="top",
                                  color=colorName[0]),
        is_smooth=True,
        symbol_size=3,
        is_connect_nones=True,
        is_selected=True,
        is_clip=True,
        is_hover_animation=True,
        linestyle_opts=opts.LineStyleOpts(
            is_show=True,
            width=0.8,
            opacity=1,
            curve=1,
            type_="solid",
            color=colorName[0],
        ))  #areastyle_opts=opts.AreaStyleOpts(opacity=0.7),curve弯曲度
    .add_yaxis(
        series_name=modelName[1],
        y_axis=list(df[modelName[1]]),
        is_smooth=True,
        #    color=Faker.rand_color(),
        label_opts=opts.LabelOpts(is_show=True,
                                  position="top",
                                  font_size=7,
                                  color=colorName[1]),
        linestyle_opts=opts.LineStyleOpts(
            is_show=True,
            color=colorName[1],
            opacity=1.0,
            width=0.7,
            curve=2,
            type_="solid")).add_yaxis(
                series_name=modelName[2],
                y_axis=list(df[modelName[2]]),
                is_smooth=True,
                # color=Faker.rand_color(),
                label_opts=opts.LabelOpts(is_show=True,
                                          position="bottom",
                                          color=colorName[2],
                                          font_size=7),
                linestyle_opts=opts.LineStyleOpts(is_show=True,
                                                  color=colorName[2],
                                                  opacity=1,
                                                  width=1,
                                                  curve=1,
                                                  type_="solid"),
            )
            .add_yaxis(
        series_name=modelName[3],
        y_axis=list(df[modelName[3]]),
        is_smooth=True,
        #    color=Faker.rand_color(),
        label_opts=opts.LabelOpts(is_show=True,
                                  position="top",
                                  font_size=7,
                                  color=colorName[3]),
        linestyle_opts=opts.LineStyleOpts(
            is_show=True,
            color=colorName[3],
            opacity=1.0,
            width=1,
            curve=1,
            type_="solid"))
            .set_colors(colorName)
    # .set_series_opts(
    #     axisline_opts=opts.AxisLineOpts(linestyle_opts=opts.LineStyleOpts(
    #         color=[[0.3, "#67e0e3"], [0.7, "#37a2da"], [1, "#fd666d"]],
    #         width=300)),
    #     label_opts=opts.LabelOpts(
    #         font_size=7.0, position="top", is_show=True))
    .set_global_opts(
        title_opts=opts.TitleOpts(title="bits指数实验", subtitle="be: 0~8"),
        xaxis_opts=opts.AxisOpts(
            type_="value",
            name="bits exponent",
            is_show=True,
            is_scale=True,
            name_location='middle',
            name_gap=20,
            # min_=1,
            max_=numExp,
            min_interval=1,
            max_interval=1,
            axislabel_opts=opts.LabelOpts(margin=5, color="#00508b"),
            axisline_opts=opts.AxisLineOpts(is_show=True,
                                            is_on_zero=True,
                                            symbol=['none', 'arrow']),
            axistick_opts=opts.AxisTickOpts(
                is_show=True,
                length=1000,
                is_inside=True,
                is_align_with_label=True,
                linestyle_opts=opts.LineStyleOpts(color="#ff1f1f1f"),
            ),  # 栅栏颜色
        ),
        yaxis_opts=opts.AxisOpts(
            type_="value",
            name="RunTime(ms)",
            is_show=True,
            is_scale=False,
            name_location='middle',
            name_gap=35,
            min_=0,
            max_=26,
            min_interval=0.5,
            max_interval=2,
            axislabel_opts=opts.LabelOpts(formatter="{value}",
                                          margin=20,
                                          color="#044161"),
            axisline_opts=opts.AxisLineOpts(is_show=True,
                                            is_on_zero=True,
                                            symbol=['none', 'arrow'])),
        legend_opts=opts.LegendOpts(type_='scroll',
                                    is_show=True,
                                    pos_right=175,
                                    pos_top=167),
        tooltip_opts=opts.TooltipOpts(
            trigger="axis", axis_pointer_type="cross")  # 苗点功能 cross shadow
    ).render("./match_time.html")  #.overlap(line_mpi_p2)
)