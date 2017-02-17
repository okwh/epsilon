#include "list_controller.h"
#include <assert.h>

using namespace Shared;

namespace Sequence {

ListController::ListController(Responder * parentResponder, SequenceStore * sequenceStore, HeaderViewController * header) :
  Shared::ListController(parentResponder, sequenceStore, header),
  m_functionTitleCells{SequenceTitleCell(&m_selectableTableView, &m_parameterController),SequenceTitleCell(&m_selectableTableView, &m_parameterController),SequenceTitleCell(&m_selectableTableView, &m_parameterController)},
  m_expressionCells{SequenceExpressionCell(&m_selectableTableView),SequenceExpressionCell(&m_selectableTableView),SequenceExpressionCell(&m_selectableTableView)},
  m_parameterController(ListParameterController(this, sequenceStore)),
  m_typeParameterController(this, sequenceStore),
  m_typeStackController(StackViewController(nullptr, &m_typeParameterController, true, KDColorWhite, Palette::PurpleDark, Palette::PurpleDark))
{
  m_selectableTableView.setDelegate(this);
}

const char * ListController::title() const {
  return "Suites";
}

KDCoordinate ListController::rowHeight(int j) {
  if (m_functionStore->numberOfFunctions() < m_functionStore->maxNumberOfFunctions() && j == numberOfRows() - 1) {
    return k_emptyRowHeight;
  }
  Sequence * sequence = ((SequenceStore *)m_functionStore)->functionAtIndex(j);
  KDCoordinate height = 0;
  KDCoordinate defaultHeight = sequence->type() == Sequence::Type::Explicite ? k_emptyRowHeight : k_emptySubRowHeight;
  if (sequence->layout() == nullptr) {
    height += defaultHeight;
  } else {
    KDCoordinate size = sequence->layout()->size().height();
    height += size + defaultHeight - KDText::stringSize(" ").height();
  }
  if ((int)sequence->type() > 0) {
    if (sequence->firstInitialConditionLayout() == nullptr) {
      height += defaultHeight;
    } else {
      KDCoordinate size = sequence->firstInitialConditionLayout()->size().height();
      height += size + defaultHeight - KDText::stringSize(" ").height();
    }
  }
  if ((int)sequence->type() > 1) {
    if (sequence->secondInitialConditionLayout() == nullptr) {
      height += defaultHeight;
    } else {
      KDCoordinate size = sequence->secondInitialConditionLayout()->size().height();
      height += size + defaultHeight - KDText::stringSize(" ").height();
    }
  }
  return height;
}

bool ListController::handleEvent(Ion::Events::Event event) {
  if (Shared::ListController::handleEvent(event)) {
    return true;
  }
  if (event == Ion::Events::OK && m_selectableTableView.selectedColumn() == 1
      && m_selectableTableView.selectedRow() == numberOfRows() - 1) {
    m_selectableTableView.dataHasChanged(true);
    app()->displayModalViewController(&m_typeStackController, 0.f, 0.f, 50, 50, 0, 50);
    return true;
  }
  return false;
}

void ListController::tableViewDidChangeSelection(SelectableTableView * t, int previousSelectedCellX, int previousSelectedCellY) {
  if (m_functionStore->numberOfFunctions() == m_functionStore->maxNumberOfFunctions() || t->selectedRow() < numberOfRows() - 1) {
    SequenceCell * myCell = (SequenceCell *)t->cellAtLocation(t->selectedColumn(), t->selectedRow());
    app()->setFirstResponder(myCell);
    if (t->selectedRow() == -1) {
      return;
    }
    if (t->selectedRow() == previousSelectedCellY) {
      SequenceCell * otherCell = (SequenceCell *)t->cellAtLocation(previousSelectedCellX, previousSelectedCellY);
      myCell->selectSubCell(otherCell->selectedSubCell());
    } else {
      if (t->selectedRow() < previousSelectedCellY) {
        myCell->selectSubCell(myCell->numberOfSubCells()-1);
      } else {
        myCell->selectSubCell(0);
      }
    }
  } else {
    if (app()->firstResponder() != t) {
      app()->setFirstResponder(t);
    }
  }
}

ListParameterController * ListController::parameterController() {
  return &m_parameterController;
}

int ListController::maxNumberOfRows() {
  return k_maxNumberOfRows;
}

TableViewCell * ListController::titleCells(int index) {
  assert(index >= 0 && index < k_maxNumberOfRows);
  return &m_functionTitleCells[index];
}

TableViewCell * ListController::expressionCells(int index) {
  assert(index >= 0 && index < k_maxNumberOfRows);
  return &m_expressionCells[index];
}

void ListController::willDisplayTitleCellAtIndex(TableViewCell * cell, int j) {
  SequenceTitleCell * myCell = (SequenceTitleCell *)cell;
  Sequence * sequence = ((SequenceStore *)m_functionStore)->functionAtIndex(j);
  myCell->setSequence(sequence);
}

void ListController::willDisplayExpressionCellAtIndex(TableViewCell * cell, int j) {
  SequenceExpressionCell * myCell = (SequenceExpressionCell *)cell;
  myCell->setSequence(((SequenceStore *)m_functionStore)->functionAtIndex(j));
}

}