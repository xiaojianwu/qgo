/***************************************************************************
 *
 * interfacehandler.cpp
 * This class deals with the tree information taken from the tree when 
 * navigating in it.
 * It is called by the BoardHandler (which does the actual navigation), and 
 * sets the interface widgets accodrding to the moves information
 *
 ***************************************************************************/


//#include "defines.h"
#include "interfacehandler.h"
#include "boardwindow.h"

/*
struct ButtonState
{
    bool navPrevVar, navNextVar, navBackward, navForward, navFirst, navStartVar, navMainBranch,
		navLast, navNextBranch, navPrevComment, navNextComment, navIntersection, editPaste, editPasteBrother; // SL added eb 11
};
*/

InterfaceHandler::InterfaceHandler(BoardWindow *bw)
{
	boardwindow = bw;
//    buttonState = new ButtonState;
	scored_flag = false;

	tabPlay  = boardwindow->getUi().tabTools->widget(0); 
	tabEdit = boardwindow->getUi().tabTools->widget(1) ;
}

InterfaceHandler::~InterfaceHandler()
{
//    delete buttonState;
}


void InterfaceHandler::clearData()
{
    // qDebug("void InterfaceHandler::clearData()");
	
	clearComment();
//	setMoveData(0, true, 0, 0, false, false, false);
//    modeButton->setOn(false);
//	  mainWidget->setToolsTabWidget(tabNormalScore);
//    mainWidget->editButtonGroup->setButton(0);
//    editTools->hide();
	boardwindow->capturesBlack->setText("0");
	boardwindow->capturesWhite->setText("0");
	
//	if (board->getGameMode() != modeObserve && 
//		board->getGameMode() != modeMatch &&
//		board->getGameMode() != modeTeach)
//	{
		boardwindow->pb_timeBlack->setText("00:00");
		boardwindow->pb_timeWhite->setText("00:00");
//	}
//    normalTools->show();
	boardwindow->scoreButton->setDown(false);
//	editPaste->setEnabled(false);
//	editPasteBrother->setEnabled(false);
	boardwindow->slider->setValue(0);
	boardwindow->slider->setMaximum(SLIDER_INIT);
	scored_flag = false;
}

/*
 * displays the informations relative to a game on the board window
 * TODO : decide wether this is here or at boardwindow level
 */
void InterfaceHandler::updateCaption(GameData *gd)
{
    // Print caption
    // example: qGo 0.0.5 - Zotan 8k vs. tgmouse 10k
    // or if game name is given: qGo 0.0.5 - Kogo's Joseki Dictionary
	boardwindow->setWindowTitle( /* QString(isModified ? "* " : "") + */
		( (gd->gameNumber != 0 && gd->gameNumber < 10000) ?
			"(" + QString::number(gd->gameNumber) + ") " : 
			QString()) + (gd->gameName.isEmpty() ?
				gd->playerWhite 
				+ (!gd->rankWhite.isEmpty() ?
					" " + gd->rankWhite : 
					QString()) 
				+ " " + QObject::tr("vs.") + " " + gd->playerBlack 
				+ (!gd->rankBlack.isEmpty() ?
					" " + gd->rankBlack : 
					QString()) :
				gd->gameName) +	"   " + QString(PACKAGE   " "  VERSION));


	bool simple = gd->rankWhite.length() == 0 && gd->rankBlack.length() == 0;
	QGroupBox *gb = boardwindow->getUi().whiteFrame;

	QString player = gd->playerWhite;
	if (simple && player == QObject::tr("White"))
		gb->setTitle(QObject::tr("White"));	
	else
	{
		// truncate to 12 characters max
		player.truncate(12);

		if (gd->rankWhite.length() != 0)
			player = QObject::tr("W") + ": " + player + " " + gd->rankWhite;
		else
			player = QObject::tr("W") + ": " + player;
		
		gb->setTitle(player);
	}

	gb = boardwindow->getUi().blackFrame;

	player = gd->playerBlack;
	if (simple && player == QObject::tr("Black"))
		gb->setTitle(QObject::tr("Black"));	
	else
	{
		// truncate to 12 characters max
		player.truncate(12);

		if (gd->rankBlack.length() != 0)
			player = QObject::tr("B") + ": " + player + " " + gd->rankBlack;
		else
			player = QObject::tr("B") + ": " + player;
		
		gb->setTitle(player);
	}
	
	//TODO set  clock

	boardwindow->getUi().komi->setText(QString().setNum(gd->komi));
	boardwindow->getUi().handicap->setText(QString().setNum(gd->handicap));
}

/*
 * displays the informations relative to a move on the board window
 */
void InterfaceHandler::setMoveData(int n, bool black, int brothers, int sons, bool hasParent, bool hasPrev, bool hasNext, int lastX, int lastY)
{
	// move number
	QString s(QObject::tr("Move") + " ");
	s.append(QString::number(n));

	// color and coordinates
	if (lastX >= 1 && lastX <= boardwindow->getBoardSize() && lastY >= 1 && lastY <= boardwindow->getBoardSize())
	{
		s.append(" (");
		s.append(black ? QObject::tr("W")+" " : QObject::tr("B")+" ");
		s.append(QString(QChar(static_cast<const char>('A' + (lastX<9?lastX:lastX+1) - 1))) +
			QString::number(boardwindow->getBoardSize()-lastY+1) + ")");
	}

	//pass move
	else if (lastX == 20 && lastY == 20)
	{
		s.append(" (");
		s.append(black ? QObject::tr("W")+" " : QObject::tr("B")+" ");
		s.append(" " + QObject::tr("Pass") + ")");
	}

	boardwindow->getUi().moveNumLabel->setText(s);
//	statusTurn->setText(" " + s.right(s.length() - 5) + " ");  // Without 'Move '
	
//	statusNav->setText(" " + QString::number(brothers) + "/" + QString::number(sons));

	// set turn information (and color on the edit button)
	s = black ? QObject::tr("Black to play") : QObject::tr("White to play");
	boardwindow->getUi().turnLabel->setText(s);
	
	boardwindow->getUi().colorButton->setIcon(black ? QIcon(":/new/prefix1/ressources/pics/stone_black.png") : QIcon(":/new/prefix1/ressources/pics/stone_white.png") );

	// sons and variatons display
	s = "";
	s.append(QString::number(brothers));
	if (brothers == 1)
		s.append(" " + QObject::tr("brother") + "\n");
	else
		s.append(" " + QObject::tr("brothers") + "\n");

	s.append(QString::number(sons));
	if (sons == 1)
		s.append(" " + QObject::tr("son"));
	else
		s.append(" " + QObject::tr("sons"));
	boardwindow->getUi().varLabel->setText(s);
	
	if (boardwindow->getGameMode() == modeNormal || boardwindow->getGameMode() == modeObserve )//|| board->getGameMode() == modeEdit)
	{
		// Update the toolbar buttons
		boardwindow->getUi().navPrevVar->setEnabled(hasPrev);
		boardwindow->getUi().navNextVar->setEnabled(hasNext);
		boardwindow->getUi().navBackward->setEnabled(hasParent);
		boardwindow->getUi().navForward->setEnabled(sons);
		boardwindow->getUi().navFirst->setEnabled(hasParent);
		boardwindow->getUi().navStartVar->setEnabled(hasParent);
		boardwindow->getUi().navMainBranch->setEnabled(hasParent);
		boardwindow->getUi().navLast->setEnabled(sons);
		boardwindow->getUi().navNextBranch->setEnabled(sons);
		boardwindow->getUi().swapVarButton->setEnabled(hasPrev);
		boardwindow->getUi().navPrevComment->setEnabled(hasParent);
		boardwindow->getUi().navNextComment->setEnabled(sons);
    		boardwindow->getUi().navIntersection->setEnabled(true);
		
		boardwindow->getUi().slider->setEnabled(true);
	}
/*	else  if (board->getGameMode() == modeObserve)
	{
		// Update the toolbar buttons
		navBackward->setEnabled(hasParent);
		navForward->setEnabled(sons);
		navFirst->setEnabled(hasParent);
		navLast->setEnabled(sons);
		navPrevComment->setEnabled(hasParent);
		navNextComment->setEnabled(sons);
		navIntersection->setEnabled(true);

		slider->setEnabled(true);

//		board->getBoardHandler()->display_incoming_move = !bool(sons);

	}
	else
		slider->setDisabled(true);
*/	
	// Update slider
	boardwindow->getUi().slider->blockSignals (TRUE);

//	int mv = boardwindow->getUi().slider->maximum();
//	int v = boardwindow->getUi().slider->value();

	if (boardwindow->getUi().slider->maximum() < n)
		  setSliderMax(n);

	// we need to be carefull with the slider :
	// normal case, slider is moved
//	if (board->getGameMode() != modeObserve ||
	// observing, but browsing (no incoming move)
//	(board->getGameMode() == modeObserve && mv >= n) ||
	// observing, but at the last move, and an incoming move occurs 
//	(board->getGameMode() == modeObserve && mv < n && v==n-1))
		boardwindow->getUi().slider->setValue(n);

	boardwindow->getUi().slider->blockSignals (FALSE);
}


/*
 * display text in the comment area
 */
void InterfaceHandler::displayComment(const QString &c)
{
//	if (board->get_isLocalGame())
//	{
		if (c.isEmpty())
			boardwindow->getUi().commentEdit->clear();
		else
			boardwindow->getUi().commentEdit->setText(c);
//	}
//	else if (!c.isEmpty())
//			commentEdit->append(c);
}


/*
 * clear the big field (offline)
 */
void InterfaceHandler::clearComment()
{
	boardwindow->commentEdit->clear();
}


/*
 * modifies the maximum value of the slider (used when a move is added)
 */
void InterfaceHandler::setSliderMax(int n)
{
	if (n < 0)
		n = 0;

	boardwindow->getUi().slider->setMaximum(n);
    	boardwindow->getUi().sliderRightLabel->setText(QString::number(n));
}

/*
 * Sets the number of prisonners on the UI
 */
void InterfaceHandler::setCaptures(float black, float white)
{

	boardwindow->getUi().capturesBlack->setText(QString::number(black));
	boardwindow->getUi().capturesWhite->setText(QString::number(white));
}

/*
 * This updates the UI with the correct layout depending on the game mode
 */
void InterfaceHandler::toggleMode(GameMode mode)
{
	
	switch (mode)
	{
		
	case modeNormal:
//		modeButton->setEnabled(true);
//		mainWidget->setToolsTabWidget(tabEdit, tabEnable);
		boardwindow->getUi().tabTools->removeTab(0) ;//setVisible(false);
		boardwindow->getUi().scoreButton->setEnabled(true);
//		scoreButton->setText(QObject::tr("Score", "button label"));
		boardwindow->getUi().passButton_2->setEnabled(true);
		boardwindow->getUi().refreshButton_2->setDisabled(true);
//		undoButton->setDisabled(true);
//		resignButton->setDisabled(true);
//		adjournButton->setDisabled(true);
//		refreshButton->setDisabled(true);
		boardwindow->getUi().commentEdit->setReadOnly(false);
		//commentEdit2->setReadOnly(true);
//		commentEdit2->setDisabled(true);
//		statusMode->setText(" " + QObject::tr("E", "Board status line: edit mode") + " ");
//		statusMark->setText(getStatusMarkText(board->getMarkType()));
		return;
		
	case modeObserve:
//		modeButton->setDisabled(true);
//		mainWidget->setToolsTabWidget(tabEdit, tabDisable);
		boardwindow->getUi().tabTools->removeTab(0) ;
		boardwindow->getUi().scoreButton_2->setDisabled(true);
//		scoreButton->setText(QObject::tr("Edit", "button label"));
		boardwindow->getUi().passButton_2->setDisabled(true);
//		boardwindow->getUi().undoButton->setDisabled(true);
//		boardwindow->getUi().resignButton->setDisabled(true);
//		boardwindow->getUi().adjournButton->setDisabled(true);
		boardwindow->getUi().refreshButton_2->setEnabled(true);
		boardwindow->getUi().commentEdit->setReadOnly(true);
//		commentEdit2->setReadOnly(false);
//		commentEdit2->setDisabled(false);
//		editCut->setEnabled(false);
//		editDelete->setEnabled(false);
//		fileNew->setEnabled(false);
//		fileNewBoard->setEnabled(false);
//		fileOpen->setEnabled(false);
//		statusMode->setText(" " + QObject::tr("O", "Board status line: observe mode") + " ");
//		statusMark->setText(getStatusMarkText(board->getMarkType()));
		return ;
		
	case modeMatch : 
//		modeButton->setDisabled(true);
//		mainWidget->setToolsTabWidget(tabEdit, tabDisable);
		boardwindow->getUi().tabTools->removeTab(1) ;
		boardwindow->getUi().scoreButton->setDisabled(true);
//		scoreButton->setText(QObject::tr("Edit", "button label"));
		boardwindow->getUi().passButton->setEnabled(true);
//		passButton->setText(QObject::tr("Pass", "button label"));
		boardwindow->getUi().undoButton->setEnabled(true);
		boardwindow->getUi().resignButton->setEnabled(true);
		boardwindow->getUi().adjournButton->setEnabled(true);
		boardwindow->getUi().refreshButton->setEnabled(true);
		boardwindow->getUi().doneButton->setEnabled(false);
		boardwindow->getUi().commentEdit->setReadOnly(true);
		boardwindow->getUi().navButtonsFrame->setEnabled(false);
//		commentEdit2->setReadOnly(false);
//		commentEdit2->setDisabled(false);
//		fileNew->setEnabled(false);
//		fileNewBoard->setEnabled(false);
//		fileOpen->setEnabled(false);
//		statusMode->setText(" " + QObject::tr("P", "Board status line: play mode") + " ");
//		statusMark->setText(getStatusMarkText(board->getMarkType()));
		return ;

	case   modeComputer :
//		modeButton->setDisabled(true);
//		mainWidget->setToolsTabWidget(tabEdit, tabDisable);
		boardwindow->getUi().tabTools->removeTab(1) ;
		boardwindow->getUi().scoreButton->setDisabled(true);
//		scoreButton->setText(QObject::tr("Edit", "button label"));
		boardwindow->getUi().passButton->setEnabled(true);
//		passButton->setText(QObject::tr("Pass", "button label"));
		boardwindow->getUi().undoButton->setEnabled(true);
		boardwindow->getUi().resignButton->setEnabled(true);
		boardwindow->getUi().adjournButton->setEnabled(false);
		boardwindow->getUi().refreshButton->setEnabled(false);
		boardwindow->getUi().doneButton->setEnabled(false);
		boardwindow->getUi().commentEdit->setReadOnly(true);
		boardwindow->getUi().navButtonsFrame->setEnabled(false);
//		commentEdit2->setReadOnly(false);
//		commentEdit2->setDisabled(false);
//		fileNew->setEnabled(false);
//		fileNewBoard->setEnabled(false);
//		fileOpen->setEnabled(false);
//		statusMode->setText(" " + QObject::tr("P", "Board status line: play mode") + " ");
//		statusMark->setText(getStatusMarkText(board->getMarkType()));
		return ;
      		
	case modeTeach:
//		board->setMode(modeTeach);
//		modeButton->setDisabled(true);
		boardwindow->getUi().tabTools->removeTab(1) ;
//		mainWidget->setToolsTabWidget(tabEdit, tabDisable);
		boardwindow->getUi().scoreButton->setDisabled(true);
//		scoreButton->setText(QObject::tr("Edit", "button label"));
		boardwindow->getUi().passButton->setEnabled(true);
//		passButton->setText(QObject::tr("Pass", "button label"));
		boardwindow->getUi().undoButton->setEnabled(true);
		boardwindow->getUi().resignButton->setEnabled(true);
		boardwindow->getUi().adjournButton->setEnabled(true);
		boardwindow->getUi().refreshButton->setEnabled(true);
		boardwindow->getUi().doneButton->setEnabled(false);
		boardwindow->getUi().commentEdit->setReadOnly(true);
		boardwindow->getUi().navButtonsFrame->setEnabled(false);
//		commentEdit2->setReadOnly(false);
//		commentEdit2->setDisabled(false);
//		fileNew->setEnabled(false);
//		fileNewBoard->setEnabled(false);
//		fileOpen->setEnabled(false);
//		statusMode->setText(" " + QObject::tr("T", "Board status line: teach mode") + " ");
//		statusMark->setText(getStatusMarkText(board->getMarkType()));
		return ;
	
	case modeReview :
//		board->setMode(modeTeach);
//		modeButton->setDisabled(true);
		boardwindow->getUi().tabTools->removeTab(1) ;
//		mainWidget->setToolsTabWidget(tabEdit, tabDisable);
		boardwindow->getUi().scoreButton->setDisabled(true);
//		scoreButton->setText(QObject::tr("Edit", "button label"));
		boardwindow->getUi().passButton->setEnabled(true);
//		passButton->setText(QObject::tr("Pass", "button label"));
		boardwindow->getUi().undoButton->setEnabled(true);
		boardwindow->getUi().resignButton->setEnabled(true);
		boardwindow->getUi().adjournButton->setEnabled(true);
		boardwindow->getUi().refreshButton->setEnabled(true);
		boardwindow->getUi().doneButton->setEnabled(false);
		boardwindow->getUi().commentEdit->setReadOnly(true);
//		commentEdit2->setReadOnly(false);
//		commentEdit2->setDisabled(false);
//		fileNew->setEnabled(false);
//		fileNewBoard->setEnabled(false);
//		fileOpen->setEnabled(false);
//		statusMode->setText(" " + QObject::tr("T", "Board status line: teach mode") + " ");
//		statusMark->setText(getStatusMarkText(board->getMarkType()));
		return ;		
//	default:
//		return modeNormal;
	}
}


void InterfaceHandler::toggleToolbarButtons(bool state)
{
//    CHECK_PTR(buttonState);
	
//    buttonState->navPrevVar = navPrevVar->isEnabled();
	boardwindow->getUi().navPrevVar->setEnabled(state);
	
//    buttonState->navNextVar = navNextVar->isEnabled();
	boardwindow->getUi().navNextVar->setEnabled(state);
	
//    buttonState->navBackward = navBackward->isEnabled();
	boardwindow->getUi().navBackward->setEnabled(state);
    
//    buttonState->navForward = navForward->isEnabled();
	boardwindow->getUi().navForward->setEnabled(state);
	
//    buttonState->navFirst = navFirst->isEnabled();
	boardwindow->getUi().navFirst->setEnabled(state);
	
//    buttonState->navStartVar = navStartVar->isEnabled();
	boardwindow->getUi().navStartVar->setEnabled(state);
	
//    buttonState->navMainBranch = navMainBranch->isEnabled();
	boardwindow->getUi().navMainBranch->setEnabled(state);
	
//    buttonState->navLast = navLast->isEnabled();
	boardwindow->getUi().navLast->setEnabled(state);
/*	
    buttonState->navNextBranch = navNextBranch->isEnabled();
    navNextBranch->setEnabled(false);
	
    buttonState->navPrevComment = navPrevComment->isEnabled();
	  navPrevComment->setEnabled(false);

    buttonState->navNextComment = navNextComment->isEnabled();
	  navNextComment->setEnabled(false);

    buttonState->navIntersection = navIntersection->isEnabled(); // added eb 111
	  navIntersection->setEnabled(false);                          // end add eb 11

    buttonState->editPaste = editPaste->isEnabled();
    editPaste->setEnabled(false);
	
    buttonState->editPasteBrother = editPasteBrother->isEnabled();
    editPasteBrother->setEnabled(false);
    
    navNthMove->setEnabled(false);
    navAutoplay->setEnabled(false);
    editCut->setEnabled(false);
    editDelete->setEnabled(false);
    navEmptyBranch->setEnabled(false);
    navCloneNode->setEnabled(false);
    navSwapVariations->setEnabled(false);
    fileImportASCII->setEnabled(false);
    fileImportASCIIClipB->setEnabled(false);
    fileImportSgfClipB->setEnabled(false);
*/
}

