<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
	xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

	<!-- 
	ЕСЛИ ВЫ РЕДАКТИРУЕТЕ ЭТОТ ДОКУМЕНТ В ECLIPSE, ОТКЛЮЧИТЕ ФОРМАТИРОВАНИЕ КОММЕНТАРИЕВ
	Window / Preferences / XML / XML Files / Editor
	
	Снимите флажок Format comments
	-->

	<!--
	Хотя файл XSL допускает передачу значений в элемент/атрибут/параметр 
	в формате 
	
	<xsl:attribute>value</xsl:attribute> 
	
	но при форматировании этого документа будут добавлены отступы и переносы,
	которые попадут в результат 
	
	<xsl:attribute> value </xsl:attribute>
	
	Рекомендуется использовать внутри контейнера инструкцию 
	
	<xsl:value-of select="'value'" />
	
	Тогда при форматировании исходного документа отступы и переносы будут проигнорированы
	<xsl:attribute> 
		<xsl:value-of select="'value'" />
	</xsl:attribute> 
	
	-->

	<!-- Параметры, задаваемые программой -->

	<!-- Имя исходного файла -->
	<xsl:param name="source_file">
		<xsl:value-of select="'path'" />
	</xsl:param>

	<!-- 
	Для выборки узлов используется язык XPath. 
	Применяемые оси XPath 
	descendant::node - узлы-потомки элемента node 
	ancestor::node - узлы-предки элемента node 
	
	https://www.w3schools.com/xml/xpath_axes.asp
	-->

	<!-- Начало обработки входящего XML-документа -->
	<xsl:template match="/extract_cadastral_plan_territory">
		<xsl:element name="ShapeFile">
			<xsl:element name="FeatureType">

				<xsl:attribute name="geometry_type">
					<xsl:value-of select="'MultiPolygon'" />
				</xsl:attribute>

				<!-- Описание структуры атрибутивной таблицы -->
				<xsl:element name="Attributes">

					<xsl:call-template name="AttributeTemplate">
						<xsl:with-param name="name">
							<xsl:value-of select="'src_file'" />
						</xsl:with-param>
						<xsl:with-param name="value">
							<xsl:value-of select="'String'" />
						</xsl:with-param>
						<xsl:with-param name="use_value_as_type">
							<xsl:value-of select="true()" />
						</xsl:with-param>
					</xsl:call-template>

					<xsl:call-template name="AttributeTemplate">
						<xsl:with-param name="name">
							<xsl:value-of select="'cad_num'" />
						</xsl:with-param>
						<xsl:with-param name="value">
							<xsl:value-of select="'String'" />
						</xsl:with-param>
						<xsl:with-param name="use_value_as_type">
							<xsl:value-of select="true()" />
						</xsl:with-param>
					</xsl:call-template>

					<xsl:call-template name="AttributeTemplate">
						<xsl:with-param name="name">
							<xsl:value-of select="'cad_qrtr'" />
						</xsl:with-param>
						<xsl:with-param name="value">
							<xsl:value-of select="'String'" />
						</xsl:with-param>
						<xsl:with-param name="use_value_as_type">
							<xsl:value-of select="true()" />
						</xsl:with-param>
					</xsl:call-template>

					<xsl:call-template name="AttributeTemplate">
						<xsl:with-param name="name">
							<xsl:value-of select="'area'" />
						</xsl:with-param>
						<xsl:with-param name="value">
							<xsl:value-of select="'Number'" />
						</xsl:with-param>
						<xsl:with-param name="use_value_as_type">
							<xsl:value-of select="true()" />
						</xsl:with-param>
					</xsl:call-template>
				</xsl:element>

			</xsl:element>
			<xsl:element name="Features">
				<xsl:apply-templates
					select="descendant::land_record" />
			</xsl:element>
		</xsl:element>
	</xsl:template>

	<xsl:template match="land_record">
		<xsl:element name="Feature">
			<xsl:element name="Attributes">

				<xsl:call-template name="AttributeTemplate">
					<xsl:with-param name="name">
						<xsl:value-of select="'src_file'" />
					</xsl:with-param>
					<xsl:with-param name="value">
						<!-- Значение параметра -->
						<xsl:value-of select="$source_file" />
					</xsl:with-param>
				</xsl:call-template>

				<xsl:call-template name="AttributeTemplate">
					<xsl:with-param name="name">
						<xsl:value-of select="'cad_num'" />
					</xsl:with-param>
					<xsl:with-param name="value">
						<!-- Значение находится внутри относительно текущего элемента -->
						<xsl:value-of select="object/common_data/cad_number" />
					</xsl:with-param>
				</xsl:call-template>

				<xsl:call-template name="AttributeTemplate">
					<xsl:with-param name="name">
						<xsl:value-of select="'cad_qrtr'" />
					</xsl:with-param>
					<xsl:with-param name="value">
						<xsl:value-of
							select="ancestor::cadastral_block/cadastral_number" />
					</xsl:with-param>
				</xsl:call-template>

				<xsl:call-template name="AttributeTemplate">
					<xsl:with-param name="name">
						<xsl:value-of select="'area'" />
					</xsl:with-param>
					<xsl:with-param name="value">
						<xsl:value-of select="params/area/value" />
					</xsl:with-param>
				</xsl:call-template>
			</xsl:element>
			<xsl:element name="Geometry">
				<xsl:apply-templates
					select="descendant::spatials_elements" />
			</xsl:element>
		</xsl:element>
	</xsl:template>

	<!--
	 Игнорируем в шаблонах элементы "contour" и "entity_spatial".
	 Обработка "entity_spatial" потребуется, если будет необходимо обрабатывать
	 систему координат в элементе "sk_id".
	 
	 Простые полигоны состоят из одной оболочки (shell)
	 -->

	<xsl:template match="spatials_elements">
		<xsl:element name="Shell">
			<xsl:apply-templates
				select="descendant::spatial_element[1]" />
			<xsl:choose>
				<xsl:when test="count(descendant::spatial_element) &gt; 1">
					<xsl:element name="Holes">
						<xsl:apply-templates select="descendant::spatial_element[position() &gt; 1]" mode="hole"/>
					</xsl:element>
				</xsl:when>
			</xsl:choose>
		</xsl:element>
	</xsl:template>
	
	<xsl:template match="spatial_element">
		<xsl:apply-templates select="descendant::ordinate" />
	</xsl:template>
	
	<xsl:template match="spatial_element" mode="hole">
		<xsl:element name="Hole">
			<xsl:apply-templates select="descendant::ordinate" />
		</xsl:element>
	</xsl:template>

	<xsl:template match="ordinate">
		<xsl:element name="Coordinate">
			<!-- Обратите внимание! Уточните - нужно ли менять местами координаты. -->
			<xsl:attribute name="x">
				<xsl:value-of select="descendant::x" />
			</xsl:attribute>
			<xsl:attribute name="y">
				<xsl:value-of select="descendant::y" />
			</xsl:attribute>
			<xsl:attribute name="ord_number">
				<xsl:value-of select="descendant::ord_nmb" />
			</xsl:attribute>
		</xsl:element>
	</xsl:template>

	<xsl:template name="AttributeTemplate">
		<xsl:param name="name" />
		<xsl:param name="value" />
		<xsl:param name="use_value_as_type" select="false()" />

		<xsl:element name="Attribute">
			<xsl:attribute name="name">
				<xsl:value-of select="$name" />
			</xsl:attribute>
			<xsl:choose>
				<xsl:when test="$use_value_as_type = true()">
					<xsl:attribute name="type">
						<xsl:value-of select="$value" />
					</xsl:attribute>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$value" />
				</xsl:otherwise>
			</xsl:choose>
		</xsl:element>
	</xsl:template>

</xsl:stylesheet>