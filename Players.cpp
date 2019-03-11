///
//    Author: Richard Opsal
//
//   Purpose: Implementation of regions for the "31" board.
//            CS3350 demonstration program.
//
//  Location: Qt.Demo/Topaz31
//  $RCSfile$
//
//   Started: 2009/12/16
// $Revision$
//     $Date$
///

///
//  C / C++ / STL includes.
///

#include <cstdlib>
#include <algorithm>


///
//  Qt includes.
///

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QList>
#include <QListIterator>
#include <QStack>
#include <QTimer>

#include <QBrush>
#include <QColor>
#include <QPen>


///
//  Local includes.
///

#include "Card.h"
#include "CardDeck.h"
#include "Players.h"


///
// Constructor Player :
///

Player:: Player(Dealer & rdealerIn, QGraphicsScene & rsceneIn, QGraphicsItem * pparentIn)

        : QGraphicsRectItem( pparentIn ),
          fOnYourHonour( true ),
          rcdeck( * new CardDeck ),
          rdealer( rdealerIn ),
          rscene ( rsceneIn  ),
          brushActive  ( QColor("#FBE9E7") ),                           // Deep orange 50
          brushKnock   ( QColor("#E53935") ),
          brushKnockHL ( QColor("#D32F2F"),  Qt::DiagCrossPattern ),    // Red 700
          brushLostHL  ( QColor("#AA00FF"),  Qt::FDiagPattern ),        // Purple A700
          brushLostOL  ( QColor("#4A148C") ),                           // Purple 900
          brushWinnerHL( QColor("#7986CB"),  Qt::Dense5Pattern ),       // Indigo 300
          brushWinnerOL( QColor("#F50057") ),                           // Pink A400
          brushHas31HL ( QColor("#FFD54F"),  Qt::CrossPattern ),        // Amber 300
          brushHas31OL ( QColor("#FF6F00") ),                           // Amber 900
          brushOutline ( QColor("#1A237E") )                            // Indigo 900

    {
    static const char * ATOKENS[] =
        {
        ":/images/gcoin1.png",
        ":/images/gcoin2.png",
        ":/images/emerald.png",
        ":/images/garnet.png",
        ":/images/ruby.png",
        ":/images/sapphire.png",
        ":/images/topaz.png"
        } ;
    static const unsigned CTOKENS = sizeof(ATOKENS) / sizeof(const char *) ;


    unsigned cLives = CLIVES ;
    while (cLives--)
        {
        QPixmap pixmapToken(ATOKENS[rand()%CTOKENS], "PNG") ;
        QGraphicsPixmapItem * ptoken = new QGraphicsPixmapItem(pixmapToken) ;
        ptoken->setParentItem(this) ;
        ptoken->setScale(0.5) ;
        ptoken->hide() ;
        listTokens.push_back( ptoken ) ;
        }


    Player::showPlayerNormal() ;
    }


///
// Destructor Player :
///

Player:: ~Player()
    {
    delete &rcdeck ;
    }


///
//  Routine Player:: appendCard() :
//
//      Add a card to the player's pile.
///

void Player:: appendCard(Card * pcard)
    {
    rcdeck.appendCard(pcard) ;
    pcard->setParentItem(this) ;
    pcard->setZValue( rcdeck.count() ) ;
    }


///
//  Routine Player:: removeCard() :
//
//      Removed cards go to Dealer's discard pile.
///

void Player:: removeCard(Card * pcard)
    {
    rscene.removeItem( pcard ) ;
    rcdeck.removeCard( pcard ) ;
    rcdeck.zTheDeck() ;
    rdealer.appendDiscard( pcard ) ;
    }


///
//  Routine Player:: initializeTokens() :
//
//      Shuffles and places tokens in region.
///

void Player:: initializeTokens(unsigned clives)
    {
    // Randomly display our life tokens.
    std::random_shuffle( listTokens.begin(), listTokens.end() ) ;

    // Randomly adjust rotation on each token.
    // Trying for a "casual" look with the tokens.
//  foreach(QGraphicsPixmapItem * ptoken, listTokens)
//      {
//      signed angle = (rand()%5) * (rand()%2 ? -1 : 1) ;
//      ptoken -> setRotation( angle ) ;
//      }

    // Get one token from list.  They are all the same size.
    QGraphicsPixmapItem * ptoken = listTokens.at(0) ;

    // Parent rectangle details for placement calc.
    QRectF rectThis  = Player:: boundingRect() ;
    QRectF rectToken = ptoken-> boundingRect() ;

    // Adjust for scaling factor.
    rectToken.setWidth (rectToken.width()  * ptoken->scale()) ;
    rectToken.setHeight(rectToken.height() * ptoken->scale()) ;

    // Position offset delta.
    unsigned delta = (rectThis.height() - rectToken.height()*listTokens.count()) ;
    delta /= 5 ;

    // Starting offsets for tokens.
    unsigned xoffset = rectThis.width() - rectToken.width() - delta*3 ;
    unsigned yoffset = delta ;

    // Position the token.
    foreach(ptoken, listTokens)
        {
        ptoken->setPos(rectThis.x()+xoffset,rectThis.y()+yoffset) ;
        yoffset += delta + rectToken.height() ;
        (clives) ? ptoken->show() : ptoken->hide() ;
        (clives) ? clives-- : clives ;
        }

    // Reset the "on-your-honour" flag.
    fOnYourHonour = true ;
    }


///
//  Routine Player:: removeAToken() :
//
//      Hide another token.  If in 'on-your-honour' state, then clear.
///

void Player:: removeAToken()
    {
    fOnYourHonour = false ;
    QListIterator<QGraphicsPixmapItem *> it(listTokens);
    it.toBack();
    while ( it.hasPrevious() )
        {
        // Extract pointer to token.
        QGraphicsPixmapItem * ptoken = it.previous() ;

        // If token already hidden, then try the next one.
        if (false == ptoken->isVisible())
            continue ;

        ptoken->hide() ;
        fOnYourHonour = true ;
        break ;
        }

    // Indicate, graphically, who lost.
    showPlayerLost() ;
    }


///
//  Function Player:: isPlayerActive() :
//
//      Return the 'on-your-honour' flag.  Either tokens still visible or
//      really in  'on-your-honour' state.
///

bool Player:: isPlayerActive()
    {
    return fOnYourHonour ;
    }


///
//  Routine Player:: doThirtyOneCheck() :
///

void Player:: doThirtyOneCheck()
    {
    if (CardDeck::C31SUM == rcdeck.scoreTheHand())
        emit turnCompleted( this, Player::Thirty_one ) ;
    }


///
//  Routine Player:: showPlayerActive() :
///

void Player:: showPlayerActive ( bool fShowActive )
    {
    (fShowActive)
        ? QGraphicsRectItem::setBrush( brushActive )
        : QGraphicsRectItem::setBrush( brushNone ) ;
    }


///
//  Routine Player:: showPlayerKnocked() :
///

void Player:: showPlayerKnocked( bool fShowKnocked )
    {
    (fShowKnocked)
        ? QGraphicsRectItem::setPen( QPen(brushKnock  , cpelsKnock ) )
        : QGraphicsRectItem::setPen( QPen(brushOutline, cpelsNormal) ) ;

    if (fShowKnocked)
        QGraphicsRectItem::setBrush( brushKnockHL ) ;
    }


///
//  Routine Player:: showPlayerLost() :
///

void Player:: showPlayerLost()
    {
    QGraphicsRectItem::setPen( QPen(brushLostOL, cpelsLost) ) ;
    QGraphicsRectItem::setBrush( brushLostHL ) ;
    }


///
//  Routine Player:: showPlayerWon() :
///

void Player:: showPlayerWon()
    {
    QGraphicsRectItem::setPen( QPen(brushWinnerOL, cpelsKnock+2 ) ) ;
    QGraphicsRectItem::setBrush( brushWinnerHL ) ;
    }


///
//  Routine Player:: showPlayerHas31() :
///

void Player:: showPlayerHas31()
    {
    QGraphicsRectItem::setPen( QPen(brushHas31OL, cpelsKnock+1 ) ) ;
    QGraphicsRectItem::setBrush( brushHas31HL ) ;
    }



///                                                                         ///
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  //
///                                                                         ///


///
// Constructor NpcPlayer :
///

NpcPlayer:: NpcPlayer(Dealer & rdealerIn, QGraphicsScene & rsceneIn, QGraphicsItem * pparentIn)

        : Player( rdealerIn, rsceneIn, pparentIn )

    {
    NpcPlayer::turnaction = Player::Draw ;
    }


///
// Destructor NpcPlayer :
///

NpcPlayer:: ~NpcPlayer()
    {
    }


///
//  Routine NpcPlayer:: appendCard() :
//
//      Add a card to the player's pile.
///

void NpcPlayer:: appendCard(Card * pcard)
    {
    // Have our ancestor do basics.
    Player::appendCard(pcard) ;

    // We're the computer player, so always hide the card.
    pcard->setCardFaceUp(false) ;

    // Parent rectangle details for placement calc.
    QRectF rectThis = NpcPlayer:: boundingRect() ;
    QRectF rectCard = pcard->boundingRect() ;

    // Position offset delta.
    unsigned delta = (rectThis.height() - rectCard.height()) ;
    delta >>= 1 ;

    // Count of cards determines position.
    unsigned count = rcdeck.count() ;
    unsigned xoffset = delta + (delta * 1.8 * (count-1)) ;

    // Position the card.
    pcard->setPos(rectThis.x()+xoffset,rectThis.y()+delta) ;
    }


///
//  Routine NpcPlayer:: removeCard() :
///

void NpcPlayer:: removeCard(Card * pcard)
    {
    // Parent rectangle details for placement calc.
    // All cards same size, so okay to use one we are removing.
    QRectF rectThis = NpcPlayer:: boundingRect() ;
    QRectF rectCard = pcard->boundingRect() ;

    // Have our ancestor do basics.
    Player::removeCard(pcard) ;

    // Position offset delta.
    unsigned delta = (rectThis.height() - rectCard.height()) ;
    delta >>= 1 ;

    // Need to reposition the cards.
    unsigned count = 0 ;
    foreach (Card * pcard, rcdeck.cardList)
        {
        // Card storage order determines position.
        unsigned xoffset = delta + (delta * 1.8 * (count++)) ;

        // Position the card.
        pcard->setPos(rectThis.x()+xoffset,rectThis.y()+delta) ;
        }
    }


///
//  Routine NpcPlayer:: setCardsFaceUp() :
///

void NpcPlayer:: setCardsFaceUp(bool fShowFace)
    {
    // Show face or back of all cards.
    foreach (Card * pcard, rcdeck.cardList)
        pcard->setCardFaceUp(fShowFace) ;
    }


///
//  Slot Routine NpcPlayer:: doPlayerTurn() :
///

void NpcPlayer:: doPlayerTurn

        (
        Player *        pplayerIn,  // IN - Which player?
        bool        fKnockRoundIn,  // IN - Are we in a knock round?
        unsigned          roundIn,  // IN - Which round of play?
        unsigned       cPlayersIn,  // IN - Count of active players.
        PlayLevel     playLevelIn   // IN - Difficult of play indicator.
        )

    {
    // Special check for "31" deal.
    if (0 == pplayerIn)
        {
        doThirtyOneCheck() ;
        }

    // Possible this isn't our turn!
    else if (this == pplayerIn)
        {
        // Change color to active.
        Player::showPlayerActive() ;

        // Score it two ways, with and without discard.
        unsigned scoreHand    = rcdeck.scoreTheHand() ;
        unsigned scoreDiscard = rcdeck.scoreTheHand( rdealer.topDiscard() ) ;

        bool fKnocked = false ;
        if (!fKnockRoundIn)
            {
            // Algorithm for deciding on knock action.
            unsigned scoreTest = 24 - cPlayersIn ;
            scoreTest += roundIn * ((rand() % 2)+1) ;
            scoreTest -= 4 - playLevelIn ;
            if (scoreHand >= scoreTest)
                {
                fKnocked = true ;
                turnaction = Player::Knock ;
                Player::showPlayerKnocked() ;
                }
            }

        // If knock round, or didn't knock, maximize our choices.
        if (fKnockRoundIn || !fKnocked)
            {
            if (scoreDiscard > (scoreHand+3))
                {
                Card * pcard = rdealer.topDiscard() ;
                rdealer.removeDiscard( pcard ) ;
                NpcPlayer::appendCard( pcard ) ;
                }
            else
                {
                Card * pcard = rdealer.topDrawCard() ;
                rdealer.removeDrawCard( pcard ) ;
                NpcPlayer::appendCard( pcard ) ;
                }

            Card * pcard = rcdeck.lowestCardInHand() ;
            NpcPlayer::removeCard( pcard ) ;

            // Normal draw or possible "31" hand.
            unsigned sum = rcdeck.scoreTheHand() ;
            turnaction = (sum == CardDeck::C31SUM) ? Player::Thirty_one : Player::Draw ;
            }

        // Do a pause before next player gets their turn.
        QTimer::singleShot((rand() % 750) + 1000, this, SLOT(doPlayerDelay())) ;
        }
    }


///
//  Slot Routine NpcPlayer:: doRoundStarting() :
///

void NpcPlayer:: doRoundStarting()
    {
    QGraphicsScene * pscene = QGraphicsRectItem::scene() ;
    if (pscene)
        {
        QGraphicsScene & rscene = *pscene ;
        foreach(Card * pcard, rcdeck.cardList)
            rscene.removeItem( pcard ) ;
        }

    Player:: clearTheDeck() ;

    // Clear out region color highlighting.
    Player::showPlayerNormal() ;
    }


///
//  Slot Routine NpcPlayer:: doRoundCompleted() :
///

void NpcPlayer:: doRoundCompleted()
    {
    // Show our hand.
    setCardsFaceUp(true) ;

    // Edit here - Show our score!
    }


///
//  Slot Routine NpcPlayer:: doGameStarting() :
///

void NpcPlayer:: doGameStarting(unsigned clives)
    {
    Player::initializeTokens(clives) ;
    }


///
//  Slot Routine NpcPlayer:: doGameCompleted() :
///

void NpcPlayer:: doGameCompleted(Player * pplayer)
    {
    if (this == pplayer)
        Player::showPlayerWon() ;
    }


///
//  Slot Routine NpcPlayer:: doPlayerDelay() :
//
//      This routine catches timer single shot action.
//      Used to slow down game play.
///

void NpcPlayer:: doPlayerDelay()
    {
    // Change color to inactive.
    (Knock == turnaction)
        ? Player::showPlayerKnocked()
        : Player::showPlayerActive( false ) ;

    // Signal we've completed our play.  Go on to next player.
    emit turnCompleted( this, turnaction ) ;
    }


///                                                                         ///
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  //
///                                                                         ///


///
// Constructor HmnPlayer :
///

HmnPlayer:: HmnPlayer(Dealer & rdealerIn, QGraphicsScene & rsceneIn, QGraphicsItem * pparentIn)

        : Player( rdealerIn, rsceneIn, pparentIn ),
          fMyTurn( false )

    {
    }


///
// Destructor HmnPlayer :
///

HmnPlayer:: ~HmnPlayer()
    {
    }


///
//  Routine HmnPlayer:: appendCard() :
//
//      Add a card to the player's pile.
///

void HmnPlayer:: appendCard(Card * pcard)
    {
    // Have our ancestor do basics.
    Player::appendCard(pcard) ;

    // We're the human player, so always show the card.
    pcard->setCardFaceUp(true) ;

    // Parent rectangle details for placement calc.
    QRectF rectThis = HmnPlayer:: boundingRect() ;
    QRectF rectCard = pcard->boundingRect() ;

    // Position offset delta.
    unsigned delta = (rectThis.height() - rectCard.height()) ;

    // Count of cards determines position.
    unsigned count = rcdeck.count() ;
    unsigned xoffset = delta + (delta*0.6 + rectCard.width()) * (count-1) ;

    // Position the card.
    pcard->setPos(rectThis.x()+xoffset,rectThis.y()+(delta>>1)) ;
    }


///
//  Routine HmnPlayer:: removeCard() :
///

void HmnPlayer:: removeCard(Card * pcard)
    {
    // Parent rectangle details for placement calc.
    // All cards same size, so okay to use one we are removing.
    QRectF rectThis = HmnPlayer:: boundingRect() ;
    QRectF rectCard = pcard->boundingRect() ;

    // Have our ancestor do basics.
    Player::removeCard(pcard) ;

    // Position offset delta.
    unsigned delta = (rectThis.height() - rectCard.height()) ;

    // Edit here - Re-organize cards for easier reading.
    // Need to reposition the cards.
    unsigned count = 0 ; ;
    foreach (Card * pcard, rcdeck.cardList)
        {
        // Card storage order determines position.
        unsigned xoffset = delta + (delta*0.6 + rectCard.width()) * (count++) ;

        // Position the card.
        pcard->setPos(rectThis.x()+xoffset,rectThis.y()+(delta>>1)) ;
        }
    }


///
//  Slot Routine HmnPlayer:: doPlayerTurn() :
///

void HmnPlayer:: doPlayerTurn

        (
        Player *        pplayerIn,  // IN - Which player?
        bool        fKnockRoundIn,  // IN - Are we in a knock round?
        unsigned          roundIn,  // IN - Which round of play?         Ignored!
        unsigned       cPlayersIn,  // IN - Count of active players.
        PlayLevel     playLevelIn   // IN - Difficult of play indicator. Ignored!
        )

    {
    // Need to save this state.
    fKnockRound = fKnockRoundIn ;

    // Special check for "31" deal.
    if (0 == pplayerIn)
        {
        doThirtyOneCheck() ;
        }

    // Possible this isn't our turn!
    else if (fMyTurn = (this == pplayerIn))
        {
        // Setup dealer cards for clicking.
        wireDealer() ;

        // Change color to active.
        Player::showPlayerActive() ;
        }
    }


///
//  Slot Routine HmnPlayer:: doRoundStarting() :
///

void HmnPlayer:: doRoundStarting()
    {
    QGraphicsScene * pscene = QGraphicsRectItem::scene() ;
    if (pscene)
        {
        QGraphicsScene & rscene = *pscene ;
        foreach(Card * pcard, rcdeck.cardList)
            rscene.removeItem( pcard ) ;
        }

    Player:: clearTheDeck() ;

    // Clear out region color highlighting.
    Player::showPlayerNormal() ;
    }


///
//  Slot Routine HmnPlayer:: doRoundCompleted() :
///

void HmnPlayer:: doRoundCompleted()
    {
    // Edit here - Show our score!
    }


///
//  Slot Routine HmnPlayer:: doGameStarting() :
///

void HmnPlayer:: doGameStarting(unsigned clives)
    {
    Player::initializeTokens(clives) ;
    }


///
//  Slot Routine HmnPlayer:: doGameCompleted() :
///

void HmnPlayer:: doGameCompleted(Player * pplayer)
    {
    if (this == pplayer)
        Player::showPlayerWon() ;
    }


///
//  Slot Routine HmnPlayer:: doKnockAction() :
///

void HmnPlayer:: doKnockAction()
    {
    // Possible this isn't our turn or somebody else already knocked.
    if (!fMyTurn || fKnockRound)
        return ;
    fMyTurn = false ;
    unwireDealer() ;

    // Indicate we're the knock player.
    Player::showPlayerActive(false) ;
    Player::showPlayerKnocked() ;

    // Nothing else to do.
    emit turnCompleted( this, Player::Knock ) ;
    }


///
//  Slot Routine HmnPlayer:: doDrawFromDiscard() :
///

void HmnPlayer:: doDrawFromDiscard()
    {
    // Possible this isn't our turn!
    if (!fMyTurn)
        return ;
    fMyTurn = false ;
    unwireDealer() ;

    Card * pcard = rdealer.topDiscard() ;

    rdealer.removeDiscard( pcard ) ;
    HmnPlayer:: appendCard(pcard) ;

    wireMyCards() ;
    }


///
//  Slot Routine HmnPlayer:: doDrawFromDeck() :
///

void HmnPlayer:: doDrawFromDeck()
    {
    // Possible this isn't our turn!
    if (!fMyTurn)
        return ;
    fMyTurn = false ;
    unwireDealer() ;

    Card * pcard = rdealer.topDrawCard() ;

    rdealer.removeDrawCard( pcard ) ;
    HmnPlayer:: appendCard( pcard ) ;

    // Wire cards so we can click from hand to discard.
    wireMyCards() ;
    }


///
//  Slot Routine HmnPlayer:: doDiscardFromHand() :
///

void HmnPlayer:: doDiscardFromHand(Card * pcard)
    {
    // Disconnect the hand cards.  Make them unselectable.
    unwireMyCards() ;

    // Remove specified card.
    HmnPlayer::removeCard( pcard ) ;
    unsigned sum = rcdeck.scoreTheHand() ;

    // No longer active player.
    Player::showPlayerActive(false) ;

    // Normal draw or possible "31" hand.
    emit turnCompleted( this, (sum == CardDeck::C31SUM) ? Player::Thirty_one : Player::Draw ) ;
    }


///
//  Routine HmnPlayer:: wireMyCards() :
///

void HmnPlayer:: wireMyCards()
    {
    foreach(Card * pcard, rcdeck.cardList)
        connect(pcard, SIGNAL(cardClicked(Card *)), this, SLOT(doDiscardFromHand(Card *))) ;
    }


///
//  Routine HmnPlayer:: wireMyCards() :
///

void HmnPlayer:: unwireMyCards()
    {
    foreach(Card * pcard, rcdeck.cardList)
        disconnect(pcard, 0, 0, 0) ;
    }


///
//  Routine HmnPlayer:: wireDealer() :
//
//      Signal / slot wiring for top cards from deal and discard decks.
///

void HmnPlayer:: wireDealer()
    {
    // Only wiring top cards from dealer.
    Card * pcardDiscard = rdealer.topDiscard() ;
    Card * pcardDraw    = rdealer.topDrawCard() ;

    // Signals for processing player turns.
    connect( pcardDiscard, SIGNAL(cardClicked(Card *)), this, SLOT(doDrawFromDiscard()) ) ;
    connect( pcardDraw,    SIGNAL(cardClicked(Card *)), this, SLOT(doDrawFromDeck   ()) ) ;
    }


///
//  Routine HmnPlayer:: unwireDealer() :
//
//      Signal / slot wiring for top cards from deal and discard decks.
///

void HmnPlayer:: unwireDealer()
    {
    // Signals for processing player turns.
    disconnect(rdealer.topDiscard(), 0, 0, 0) ;
    disconnect(rdealer.topDrawCard(), 0, 0, 0) ;
    }


///                                                                         ///
//  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  //
///                                                                         ///


///
// Constructor Dealer :
///

Dealer:: Dealer(QGraphicsItem * pparentIn)

        : QGraphicsRectItem( pparentIn  ),
          rcdeckDraw   ( * new CardDeck ),
          rcdeckDiscard( * new CardDeck ),
          brushFill    ( QColor("#B0BEC5") ),       // Blue grey 200
          brushOutline ( QColor("#B71C1C") )        // Red 900

    {
    QGraphicsRectItem::setPen( QPen(brushOutline, cpelsNormal) ) ;
    QGraphicsRectItem::setBrush( brushFill ) ;
    }


///
// Destructor Dealer :
///

Dealer:: ~Dealer()
    {
    delete &rcdeckDraw ;
    delete &rcdeckDiscard ;
    }


///
//  Routine Dealer:: appendDrawCard() :
//
//      Add a card to the player's pile.
///

void Dealer:: appendDrawCard(Card * pcard)
    {
    // Save the card in draw deck.
    rcdeckDraw.appendCard(pcard) ;
    pcard->setParentItem(this) ;

    // We're the draw card, so always hide.
    pcard->setCardFaceUp(false) ;

    // Parent rectangle details for placement calc.
    QRectF rectThis = Dealer:: boundingRect() ;
    QRectF rectCard = pcard->boundingRect() ;

    // Position offset delta.
    unsigned delta = (rectThis.height() - rectCard.height()) ;

    // Position the card.
    pcard->setPos(rectThis.x()+(delta>>1),rectThis.y()+(delta>>1)) ;
    }


///
//  Routine Dealer:: removeDrawCard() :
///

void Dealer:: removeDrawCard(Card * pcard)
    {
    QGraphicsScene * pscene = QGraphicsRectItem::scene() ;
    if (pscene)
        {
        QGraphicsScene & rscene = *pscene ;
        rscene.removeItem( pcard ) ;
        }

    rcdeckDraw.removeCard( pcard ) ;
    }


///
//  Routine Dealer:: appendDiscard() :
//
//      Add a card to the player's pile.
//
//      2017-01-15 : RBO : Display last four discard cards.
///

void Dealer:: appendDiscard(Card * pcard)
    {
    // Stack used to display last three cards.
    QStack<Card*> cardStack ;
    cardStack.push(pcard) ;

    // Add three more cards to stack.
    for (unsigned times=3 ; times ; --times)
        {
        if (!rcdeckDiscard.isEmpty() )
            {
            cardStack.push(rcdeckDiscard.topCard()) ;
            rcdeckDiscard.removeTopCard() ;
            }
        }

    // Parent rectangle details for placement calc.
    QRectF rectThis = Dealer:: boundingRect() ;
    QRectF rectCard = pcard->boundingRect() ;

    // Position offset delta.
    unsigned delta = (rectThis.height() - rectCard.height()) ;
    unsigned xoffset = rectThis.width() / 2 - delta * 0.9 ;

    // Put cards back in discard pile.
    while (!cardStack.isEmpty())
        {
        // Grab the top card.
        Card * pcard = cardStack.pop() ;

        // Save the card in draw deck.
        rcdeckDiscard.appendCard(pcard) ;
        pcard->setParentItem(this) ;

        // We're in the discard pile, so show card.
        pcard->setCardFaceUp(true) ;

        // Position the card.
        pcard->setPos(rectThis.x()+xoffset,rectThis.y()+(delta>>1)) ;

        // Adjust xoffset for next card.
        xoffset += delta * 0.9 ;
        }
    }


///
//  Routine Dealer:: removeDiscard() :
///

void Dealer:: removeDiscard(Card * pcard)
    {
    QGraphicsScene * pscene = QGraphicsRectItem::scene() ;
    if (pscene)
        {
        QGraphicsScene & rscene = *pscene ;
        rscene.removeItem( pcard ) ;
        }

    rcdeckDiscard.removeCard( pcard ) ;
    }


///
//  Routine Dealer:: topDrawCard() :
//
//      Grabs cards from discard pile if we run out of cards.
//      Owner of cards remains the same!
///

Card * Dealer:: topDrawCard()
    {
    if (rcdeckDraw.isEmpty())
        {
        CardDeck cardDeck ;
        Card *  pcard = topDiscard() ;
        rcdeckDiscard.removeTopCard() ;
        cardDeck.cardList = rcdeckDiscard.cardList ;

        // Just one card for discard pile.
        rcdeckDiscard.cardList.clear() ;
        appendDiscard( pcard ) ;

        // Shuffle temporary deck and place in draw deck.
        cardDeck.shuffleTheDeck() ;
        foreach(Card * pcard, cardDeck.cardList)
            appendDrawCard(pcard) ;
        }

    return rcdeckDraw.topCard() ;
    }


///
//  Slot Routine Dealer:: doRoundStarting() :
///

void Dealer:: doRoundStarting()
    {
    QGraphicsScene * pscene = QGraphicsRectItem::scene() ;
    if (pscene)
        {
        QGraphicsScene & rscene = *pscene ;
        foreach(Card * pcard, rcdeckDiscard.cardList)
            rscene.removeItem( pcard ) ;

        foreach(Card * pcard, rcdeckDraw.cardList)
            rscene.removeItem( pcard ) ;
        }

    Dealer:: clearTheDecks() ;
    }
