#include "lefTechLayerSpacingEolParser.h"
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/bind.hpp>
#include <boost/optional/optional_io.hpp>
#include <boost/spirit/include/qi_alternative.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>
#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/container.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <iostream>
#include <string>



namespace lefTechLayerSpacingEol {
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    namespace phoenix = boost::phoenix;
   
    using boost::spirit::qi::lit;
    using boost::spirit::ascii::string;
    using boost::spirit::ascii::space_type;
    using boost::fusion::at_c;
    
    using qi::double_;
    using qi::int_;
    // using qi::_1;
    using ascii::space;
    using phoenix::ref;
    
    void parallelEdgeParser(const boost::fusion::vector<std::string,
                                                    boost::optional<std::string> ,
                                                    double ,
                                                    double ,
                                                    boost::optional< boost::fusion::vector2<std::string, double> >,
                                                    boost::optional< boost::fusion::vector2<std::string, double> >,
                                                    boost::optional<std::string> ,
                                                    boost::optional<std::string> ,
                                                    boost::optional<std::string> ,
                                                    boost::optional<std::string> >& params
                            , odb::dbTechLayerSpacingEolRule* sc)
    {

        // Decoding struct
        const boost::optional<std::string>& isSubtractEolWidth= at_c<1>(params);
        const double& parSpace= at_c<2>(params);
        const double& parWithin= at_c<3>(params);
        const boost::optional<boost::fusion::vector2<std::string, double> >& prl= at_c<4>(params);
        const boost::optional<boost::fusion::vector2<std::string, double> >& minLength= at_c<5>(params);
        const boost::optional<std::string>& isTwoEdges= at_c<6>(params);
        const boost::optional<std::string>& isSameMetal= at_c<7>(params);
        const boost::optional<std::string>& isNonEolCornerOnly= at_c<8>(params);
        const boost::optional<std::string>& isParallelSameMask= at_c<9>(params);                                              


        // Populating Object
        sc->setPARALLELEDGE(true);
        if(isSubtractEolWidth.is_initialized()) sc->setSUBTRACTEOLWIDTH(true);
        sc->setparSpace(parSpace);
        sc->setparWithin(parWithin);
        if(prl.is_initialized()) sc->setPARPRL(true);
        if(prl.is_initialized()) sc->setparPrl(at_c<1>(prl.value()));
        if(minLength.is_initialized()) sc->setPARMINLENGTH(true);
        if(minLength.is_initialized()) sc->setparMinLength(at_c<1>(minLength.value()));
        if(isTwoEdges.is_initialized()) sc->setTWOEDGES(true);
        if(isSameMetal.is_initialized()) sc->setSAMEMETAL(true);
        if(isNonEolCornerOnly.is_initialized()) sc->setNONEOLCORNERONLY(true);
        if(isParallelSameMask.is_initialized()) sc->setPARALLELSAMEMASK(true);
        
    }


    void exceptExactParser(const boost::fusion::vector<std::string, double, double>& params, odb::dbTechLayerSpacingEolRule* sc){
       
        const double& exactWidth = at_c<1>(params);
        const double& otherWidth = at_c<2>(params);
    
        sc->setEXCEPTEXACTWIDTH(true);
        sc->setexactWidth(exactWidth);
        sc->setotherWidth(otherWidth);

    }

    void fillConcaveParser(const boost::fusion::vector<std::string, double>& params, odb::dbTechLayerSpacingEolRule* sc){

        const double& fillTriangle = at_c<1>(params);
    
        sc->setFILLCONCAVECORNER(true);
        sc->setfillTriangle(fillTriangle);
    }


    void withcutParser(const boost::fusion::vector<std::string, 
                                            boost::optional< boost::fusion::vector2<std::string, double> > ,
                                            boost::optional<std::string>,
                                            double,
                                            boost::optional< boost::fusion::vector3 <std::string, double, boost::optional<boost::fusion::vector2<std::string, double> > >  >                
                                            >& params, 
                        odb::dbTechLayerSpacingEolRule* sc){


        const boost::optional< boost::fusion::vector2<std::string, double> >& cutclass = at_c<1>(params);
        const boost::optional<std::string> &above = at_c<2>(params);
        const double &cutSpace = at_c<3>(params);
        const boost::optional< boost::fusion::vector3 <std::string, double, boost::optional<boost::fusion::vector2<std::string, double> > > > &enclosureEnd = at_c<4>(params);
        
        
        sc->setWITHCUT(true);
        if(cutclass.is_initialized()) sc->setCUTCLASS(true);
        if(cutclass.is_initialized()) sc->setcutClass(at_c<1>(cutclass.value()));
        if(above.is_initialized()) sc->setwithCutABOVE(true);
        sc->setwithCutSpace(cutSpace);
        if(enclosureEnd.is_initialized()) sc->setENCLOSUREEND(true);
        if(enclosureEnd.is_initialized()) sc->setenclosureEndWidth(at_c<1>(enclosureEnd.value()));
        if(enclosureEnd.is_initialized() && (at_c<2>(enclosureEnd.value())).is_initialized()) sc->setENCLOSUREENDWITHIN(true);
        if(enclosureEnd.is_initialized() && (at_c<2>(enclosureEnd.value())).is_initialized()){
            const double& enclosureEndWithin = at_c<1>( (at_c<2>(enclosureEnd.value())).value() );
            sc->setenclosureEndWithin(enclosureEndWithin);
        }
 
    }


    void endprlspacingParser(const boost::fusion::vector<std::string, 
                                                    double ,
                                                    std::string,
                                                    double
                                                    >& params, 
                        odb::dbTechLayerSpacingEolRule* sc){

                            
       sc->setENDPRLSPACING(true);
       sc->setendPrlSpace(at_c<1>(params) );
       sc->setPRL(true);
       sc->setendPrl(at_c<3>(params));
 
    }
    

    void endtoendspacingParser(const boost::fusion::vector<std::string, 
                                            double ,
                                            boost::optional<boost::fusion::vector2<double,double> >,
                                            boost::optional<boost::fusion::vector3<std::string,double,boost::optional<double> > >,
                                            boost::optional<boost::fusion::vector2<std::string,double> >
                                            >& params, 
                        odb::dbTechLayerSpacingEolRule* sc){

                        
                        
        const boost::optional<boost::fusion::vector2<double,double> >& twoCutSpaces = at_c<2>(params);
        const boost::optional<boost::fusion::vector3<std::string,double,boost::optional<double> > >& extension = at_c<3>(params);
        const boost::optional<boost::fusion::vector2<std::string,double> > otherendWidth = at_c<4>(params);



        sc->setENDTOEND(true);
        sc->setendToEndSpace(at_c<1>(params));
        if(twoCutSpaces.is_initialized()) sc->setoneCutSpace( at_c<0>(twoCutSpaces.value()) );
        if(twoCutSpaces.is_initialized()) sc->settwoCutSpace( at_c<1>(twoCutSpaces.value()) );

        if(extension.is_initialized()) sc->setEXTENSION(true);
        if(extension.is_initialized()) sc->setExtension( at_c<1>(extension.value()) );
        if(extension.is_initialized() && (at_c<2>(extension.value())).is_initialized() ) sc->setwrongDirExtension( (at_c<2>(extension.value())).value() );

        if(otherendWidth.is_initialized()) sc->setOTHERENDWIDTH(true);
        if(otherendWidth.is_initialized()) sc->setotherEndWidth( at_c<1>(otherendWidth.value()) );
 
    }


    void maxminlengthParser(const boost::variant< boost::fusion::vector<std::string, double>, 
                                                 boost::fusion::vector<std::string, double, boost::optional<std::string > > 
                                            >& params, 
                        odb::dbTechLayerSpacingEolRule* sc){

                    

        if(boost::get<boost::fusion::vector<std::string, double>>(&params)){
            boost::fusion::vector<std::string, double> mx = boost::get<boost::fusion::vector<std::string, double>>(params);
            sc->setMAXLENGTH(true);
            sc->setmaxLength(at_c<1>(mx));
        } else {
            boost::fusion::vector<std::string, double, boost::optional<std::string > > mn = boost::get<boost::fusion::vector<std::string, double, boost::optional<std::string > > >(params);
            sc->setMINLENGTH(true);
            sc->setminLength(at_c<1>(mn));
            if( (at_c<2>(mn)).is_initialized() ) sc->setTWOSIDES(true);
        }      
    }


    void enclosecutParser(const boost::fusion::vector< std::string,
                                               boost::optional<boost::variant<std::string, std::string> >,
                                               double,
                                               std::string,
                                               double,
                                               boost::optional<std::string>
                                            >& params, 
                        odb::dbTechLayerSpacingEolRule* sc){


        sc->setENCLOSECUT(true);
        if( (at_c<1>(params)).is_initialized() && boost::get<std::string>((at_c<1>(params)).value()) == "ABOVE" )  sc->setABOVE(true);
        else if ( (at_c<1>(params)).is_initialized() && boost::get<std::string>((at_c<1>(params)).value()) == "BELOW" )  sc->setBELOW(true);
        sc->setencloseDist(at_c<2>(params));
        sc->setCUTSPACING(true);
        sc->setcutToMetalSpace(at_c<4>(params));
        if( (at_c<5>(params)).is_initialized()) sc->setALLCUTS(true);    
    }

     void oppositeWidthParser(const boost::fusion::vector<std::string, double>& params, odb::dbTechLayerSpacingEolRule* sc){

        const double& oppositeWidth = at_c<1>(params);
    
        sc->setOPPOSITEWIDTH(true);
        sc->setoppositeWidth(oppositeWidth);
    }
    
    void concaveCornerParser(const boost::fusion::vector< std::string,
                                                            boost::optional<boost::fusion::vector<std::string, double> >,
                                                            boost::optional< boost::fusion::vector2<std::string, boost::variant<boost::fusion::vector<double, double>, double>  >  >
                                            >& params, 
                        odb::dbTechLayerSpacingEolRule* sc){

        const boost::optional<boost::fusion::vector<std::string, double> >& minlength = at_c<1>(params);
        const boost::optional< boost::fusion::vector2<std::string, boost::variant<boost::fusion::vector<double, double>, double>  >  >& minAdjlength = at_c<2>(params);                             
        

        sc->setTOCONCAVECORNER(true);
        if(minlength.is_initialized()) sc->setMINLENGTH(true);
        if(minlength.is_initialized()) sc->setminLength( at_c<1>(minlength.value()));

        if(minAdjlength.is_initialized()) sc->setMINADJACENTLENGTH(true);
        if(minAdjlength.is_initialized() && boost::get<double>(& at_c<1>(minAdjlength.value()))) sc->setminAdjLength( boost::get<double>(at_c<1>(minAdjlength.value())) );
        else if(minAdjlength.is_initialized() ){
            const boost::fusion::vector<double, double>& twoAdjLengths = boost::get<boost::fusion::vector<double, double> >(at_c<1>(minAdjlength.value()));
            sc->setminAdjLength1(at_c<0>(twoAdjLengths));
            sc->setminAdjLength2(at_c<1>(twoAdjLengths));
        }

    }

    template <typename Iterator>
    bool parse(Iterator first, Iterator last, odb::dbTechLayerSpacingEolRule* sc)
    {

        

        qi::rule<std::string::iterator, space_type> prlEdgeRule = (     string("PARALLELEDGE")
                                                                        >> -(string("SUBTRACTEOLWIDTH"))
                                                                        >> double_
                                                                        >> lit("WITHIN")
                                                                        >> double_
                                                                        >> -(string("PRL") >> double_)
                                                                        >> -(string("MINLENGTH") >> double_)
                                                                        >> -(string("TWOEDGES"))
                                                                        >> -(string("SAMEMETAL"))
                                                                        >> -(string("NONEOLCORNERONLY"))
                                                                        >> -(string("PARALLELSAMEMASK"))        
                                                                    )[boost::bind(&parallelEdgeParser, _1, sc)];
        

        
        qi::rule<std::string::iterator, space_type> exceptexactRule = ( string("EXCEPTEXACTWIDTH") 
                                                                        >> double_
                                                                        >> double_ 
                                                                    ) [boost::bind(&exceptExactParser, _1, sc)];
        
        qi::rule<std::string::iterator, space_type> fillConcaveCornerRule = ( string("FILLCONCAVECORNER") 
                                                                              >> double_ 
                                                                            )[boost::bind(&fillConcaveParser, _1, sc)];
        
        
        qi::rule<std::string::iterator, space_type> withCutRule = ( string("WITHCUT")
                                                                    >> -(string("CUTCLASS") >> double_)
                                                                    >> -(string("ABOVE"))
                                                                    >>  double_
                                                                    >> -( string("ENCLOSUREEND") >> double_ >> -(string("WITHIN") >> double_) )  
                                                                  )[boost::bind(&withcutParser, _1, sc)];
        
        
        qi::rule<std::string::iterator, space_type> endprlspacingrule = (  string("ENDPRLSPACING")
                                                                           >> double_ 
                                                                           >> string("PRL") 
                                                                           >> double_
                                                                        )[boost::bind(&endprlspacingParser, _1, sc)];
        
        
        qi::rule<std::string::iterator, space_type> endtoendspacingrule = ( string("ENDTOEND") 
                                                                            >> double_ 
                                                                            >> -(double_ >> double_)
                                                                            >> -(string("EXTENSION") >> double_ >> -(double_))
                                                                            >> -(string("OTHERENDWIDTH") >> double_) 
                                                                          )[boost::bind(&endtoendspacingParser, _1, sc)]; 
                                    


        qi::rule<std::string::iterator, space_type> maxminlengthrule = (  (string("MAXLENGTH") >> double_)
                                                                        | (string("MINLENGTH") >> double_ >> -(string("TWOSIDES")) ) 
                                                                        )[boost::bind(&maxminlengthParser, _1, sc)];



 

        qi::rule<std::string::iterator, space_type> enclosecutrule = (  string("ENCLOSECUT")
                                                                         >> -(string("ABOVE") | string("BELOW"))
                                                                         >> double_
                                                                         >> string("CUTSPACING")
                                                                         >> double_
                                                                         >> -(string("ALLCUTS"))
                                                                    )[boost::bind(&enclosecutParser, _1, sc)];


        

        qi::rule<std::string::iterator, space_type> withinRule = (      -(string("OPPOSITEWIDTH") >> double_)[boost::bind(&oppositeWidthParser, _1, sc)] 
                                                                        >> lit("WITHIN") [boost::bind(&odb::dbTechLayerSpacingEolRule::setWITHIN, sc, true)]
                                                                        >> double_[boost::bind(&odb::dbTechLayerSpacingEolRule::seteolWithin, sc, _1)]
                                                                        >> -(double_[boost::bind(&odb::dbTechLayerSpacingEolRule::setwrongDirWithin, sc, _1)])
                                                                        >> -(lit("SAMEMASK")[boost::bind(&odb::dbTechLayerSpacingEolRule::setSAMEMASK, sc, true)])
                                                                        >> -exceptexactRule 
                                                                        >> -fillConcaveCornerRule 
                                                                        >> -withCutRule 
                                                                        >> -endprlspacingrule 
                                                                        >> -endtoendspacingrule 
                                                                        >> -maxminlengthrule 
                                                                        >> -(lit("EQUALRECTWIDTH")[boost::bind(&odb::dbTechLayerSpacingEolRule::setEQUALRECTWIDTH, sc, true)])
                                                                        >> -prlEdgeRule 
                                                                        >> -enclosecutrule 
                                                                    );


        


        qi::rule<std::string::iterator, space_type> toconcavecornerrule = (
                                                                        string("TOCONCAVECORNER")
                                                                        >> -(string("MINLENGTH") >> double_)
                                                                        >> -( string("MINADJACENTLENGTH") >> ( (double_ >> double_ ) | double_ ) )
                                                                       )[boost::bind(&concaveCornerParser, _1, sc)];;

        
        
        qi::rule<std::string::iterator, space_type> tonotchlengthrule = (
                                                                            lit("TONOTCHLENGTH")[boost::bind(&odb::dbTechLayerSpacingEolRule::setTONOTCHLENGTH, sc, true)] 
                                                                            >> double_[boost::bind(&odb::dbTechLayerSpacingEolRule::setnotchLength, sc, _1)]
                                                                        );
        

        
        
        qi::rule<std::string::iterator, space_type> spacingRule = (
                                                                        lit("SPACING")
                                                                        >> (double_[boost::bind(&odb::dbTechLayerSpacingEolRule::seteolSpace, sc, _1)])
                                                                        >> lit("ENDOFLINE")
                                                                        >> double_[boost::bind(&odb::dbTechLayerSpacingEolRule::setEolwidth, sc, _1)]
                                                                        >> -(lit("EXACTWIDTH")[boost::bind(&odb::dbTechLayerSpacingEolRule::setEXACTWIDTH, sc, true)])
                                                                        >> -(lit("WRONGDIRSPACING")[boost::bind(&odb::dbTechLayerSpacingEolRule::setWRONGDIRSPACING, sc, true)] 
                                                                            >> double_[boost::bind(&odb::dbTechLayerSpacingEolRule::setwrongDirSpace, sc, _1)])
                                                                        >> (  withinRule | toconcavecornerrule | tonotchlengthrule)
                                                                            
                                                                  );

        bool r = qi::phrase_parse(first, last, spacingRule, space);
        

        if (first != last) // fail if we did not get a full match
            return false;
        return r;
    }
}


namespace odb{


dbTechLayerSpacingEolRule* lefTechLayerSpacingEolParser::parse(std::string s, dbTechLayer* layer){

    dbTechLayerSpacingEolRule* rule = dbTechLayerSpacingEolRule::create(layer);
    if(lefTechLayerSpacingEol::parse(s.begin(), s.end(), rule))
        return rule;
    else 
    {
        // should delete rule 
        return NULL;
    }

} 


}

