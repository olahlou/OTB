WRAP_CLASS("itk::OpeningByReconstructionImageFilter" POINTER)
  FOREACH(d ${WRAP_ITK_DIMS})
    FOREACH(t ${WRAP_ITK_SCALAR})
      WRAP_TEMPLATE("${ITKM_I${t}${d}}${ITKM_I${t}${d}}${ITKM_SE${d}}"    "${ITKT_I${t}${d}},${ITKT_I${t}${d}},${ITKT_SE${d}}")
    ENDFOREACH(t)
  ENDFOREACH(d)
END_WRAP_CLASS()
